
#include "AI.h"

// Function manage the detection and tracking options
float AI::get_traffic_sign(Mat& image, Mat& copy_image, Mat& edge_mask, int scale) {

	if (trafficSignDetected == true) {
		this->max_speed = this->traffic_sign_tracking(image, copy_image, edge_mask, scale);
	}
	else {
		this->max_speed = this->traffic_sign_detection(copy_image, edge_mask);
	}
	return this->max_speed;
}

// Function make camera tracking after the traffic sign
float AI::traffic_sign_tracking(Mat& image, Mat& copy_image, Mat& edge_mask, int scale) {

	string label;
	Rect boundRect;

    // If tracking update success then drawing rectangle
	if(this->tracker->update(copy_image, boundRect)){

		int x = boundRect.x * scale;
		int y = boundRect.y * scale;
		int w = boundRect.width * scale;
		int h = boundRect.height * scale;

		//int x = boundRect.x;
		//int y = boundRect.y;
		//int w = boundRect.width;
		//int h = boundRect.height;
		x = x + int(image.cols - (image.cols/ 4));
		y = y + int(image.rows / 4);
		//x = x + int(image.cols - (image.cols / 2));
		//y = y + int(image.rows / 4);

		Rect tf_boundingBox(x, y, w, h);

        this->draw_rectangle(image, tf_boundingBox, this->traffic_sign);
	}
	else{
		this->onTracking = false;
		this->trafficSignDetected = false;
	}
	return this->max_speed;
}

// Function detect traffic sign
float AI::traffic_sign_detection(Mat& image, Mat& edge_mask) {

	// Get the roi of traffic sign from the image
	//Rect roi(int(edge_mask.cols / 2), int(0), int(edge_mask.cols / 2), int(edge_mask.rows / 1));
	Rect roi(int(edge_mask.cols - (edge_mask.cols / 4)), int(edge_mask.rows / 4), int(edge_mask.cols / 4), int(edge_mask.rows / 2));

	Mat img_crop = image(roi);
	Mat mask_crop = edge_mask(roi);

    // Detect the traffic sign from roi above
	Rect boundingBox = this->getDetection(img_crop, mask_crop);

	if(boundingBox.x == 0 && boundingBox.y == 0){
        return this->max_speed;
	}
	// If traffic sign detected then start tracking
	if (!boundingBox.empty()) {

		this->onTracking = true;
		this->trafficSignDetected = true;

		// Initialize the tracker
		this->tracker = TrackerKCF::create();
        this->tracker->init(image, boundingBox);
	}

	//imshow("Crop", img_crop);
    //imshow("Edge mask", mask_crop);
	return this->max_speed;
}

// Function detect traffic sign
Rect AI::getDetection(Mat &image, Mat edge_mask) {

	Mat fragment;

	string label = "";

    bool traffic_sign;

	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	findContours(edge_mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i < contours.size(); i++) {

		int area = (int)contourArea(contours[i]);

		// Stop Sign has 1700 area with scale = 1
        cout << "Area: " << area << endl;

		if (100 < area and area < 5500) {

			fragment = image(boundingRect(contours[i]));

            //cvtColor(fragment, fragment, COLOR_BGR2GRAY);

			label = this->get_prediction2(fragment);

            traffic_sign = this->traffic_sign_translation(label);

			if (traffic_sign == true) {

				this->traffic_sign = label;
				return boundingRect(contours[i]);
			}
		}
	}
	Rect failed(0,0,0,0);
	return failed;
	//return Rect(NULL, NULL, NULL, NULL);
}

// Function translate traffic sign to a command
bool AI::traffic_sign_translation(string label){

    bool relevant_label = true;

    // Remove chae '\r' from the string label
    label.erase(std::remove(label.begin(), label.end(), '\r'), label.end());

    if( label == "Stop"){

        this->max_speed = 0.0f;
        this->traffic_sign = label;
    }
    else{
        if( label == "No entry"){

            this->max_speed = 0.0f;
            this->traffic_sign = label;
        }
        else{
            if( label == "Pedestrians"){

                this->max_speed = 0.0f;
                this->traffic_sign = label;
            }
            else{
                if( label == "Speed limit (30km/h)"){

                    this->max_speed = 0.3;
                    this->traffic_sign = label;
                }
                else{
                    if( label == "Speed limit (50km/h)"){
                        this->max_speed = 0.5;
                        this->traffic_sign = label;
                    }
                    else{
                        // Label not in the relevant bank
                        relevant_label = false;
                    }
                }
            }
        }
    }
    return relevant_label;
}

// Function doing the model prediction
string AI::get_prediction(Mat& img) {

	string label = "";
    std::vector<std::string> lbl2;
    std::vector<std::__cxx11::basic_string<char>> lbl;

    int f;
    int In;
    Mat frame;
    Mat image;
    chrono::steady_clock::time_point Tbegin, Tend;

    int model_width;
    int model_height;
    int model_channels;

//  can be class variabels in h file
//    std::vector<std::string> Labels;
    std::unique_ptr<tflite::Interpreter> interpreter;

    // Load model
//   std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/user/CodeBlocks/CarV2/CarV2/Resource/inception_v4_299_quant.tflite");
//    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/user/CodeBlocks/CarV2/CarV2/Resource/mobilenet_v1_1.0_224_quant.tflite");
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/user/CodeBlocks/CarV2/CarV2/Resource/Model_Traffic_Signs_TFLite.tflite");

    // Build the interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);

    interpreter->AllocateTensors();
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(4);      //quad core

    // Get input dimension from the input tensor metadata
    // Assuming one input only
    In = interpreter->inputs()[0];
    model_height   = interpreter->tensor(In)->dims->data[1];
    model_width    = interpreter->tensor(In)->dims->data[2];
    model_channels = interpreter->tensor(In)->dims->data[3];

    cout << "height   : "<< model_height << endl;
    cout << "width    : "<< model_width << endl;
    cout << "channels : "<< model_channels << endl;

	// Get the names
	bool result = getFileContent("/home/user/CodeBlocks/CarV2/CarV2/Resource/labels.txt");
	if(!result)
	{
        cout << "loading labels failed";
        exit(-1);
	}
    frame=imread("/home/user/CodeBlocks/CarV2/CarV2/Resource/tabby.jpeg");
    //frame=img;
    cout << "type " << frame.type() << endl;
    frame.convertTo(frame, CV_32F, 1.0 / 255, 0);
    //frame.convertTo(frame, CV_32FC3, 1.f / 255);
    //cv::normalize(img, frame, 1, 255, NORM_MINMAX, CV_8UC3);

    cout << "type " << frame.type() << endl;



    if (frame.empty()) {
        cerr << "Can not load picture!" << endl;
        exit(-1);
    }

    // copy image to input as input tensor
    cv::resize(frame, image, Size(model_width,model_height),INTER_NEAREST);
    memcpy(interpreter->typed_input_tensor<uchar>(0), image.data, image.total() * image.elemSize());

    cout << "tensors size: " << interpreter->tensors_size() << "\n";
    cout << "nodes size: " << interpreter->nodes_size() << "\n";
    cout << "inputs: " << interpreter->inputs().size() << "\n";
    cout << "outputs: " << interpreter->outputs().size() << "\n";

    Tbegin = chrono::steady_clock::now();

    interpreter->Invoke();      // run your model

    Tend = chrono::steady_clock::now();

    const float threshold = 0.001f;

    std::vector<std::pair<float, int>> top_results;

    int output = interpreter->outputs()[0];
    TfLiteIntArray* output_dims = interpreter->tensor(output)->dims;
    // assume output dims to be something like (1, 1, ... ,size)
    auto output_size = output_dims->data[output_dims->size - 1];
    cout << "output_size: " << output_size <<"\n";

    switch (interpreter->tensor(output)->type) {
        case kTfLiteFloat32:
            tflite::label_image::get_top_n<float>(interpreter->typed_output_tensor<float>(0), output_size,
                                                    5, threshold, &top_results, kTfLiteFloat32);
        break;
        case kTfLiteUInt8:
            tflite::label_image::get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0), output_size,
                                                    5, threshold, &top_results, kTfLiteUInt8);
        break;
        default:
            cerr << "cannot handle output type " << interpreter->tensor(output)->type << endl;
            exit(-1);
  }

    for (const auto& result : top_results) {
        const float confidence = result.first;
        const int index = result.second;
        cout << confidence << " : " << Labels[index] << "\n";
    }
    //calculate time
    f = chrono::duration_cast <chrono::milliseconds> (Tend - Tbegin).count();
    cout << "Process time: " << f << " mSec" << endl;

//    label = Labels[index];
    //lbl = Labels[index];
    //cout << "Label: " << lbl << endl;

	return label;
}

string AI::get_prediction2(Mat& img) {

    // Copy the gragment to image
    Mat frg;
    img.copyTo(frg);
    cv::resize(frg, frg, Size(200,200),INTER_NEAREST);
    imshow("fragment", frg);

	string label = "";
	string lbl_tf = "";
    std::vector<std::string> lbl2;
    std::vector<std::__cxx11::basic_string<char>> lbl;

    // Load model
    //std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/user/CodeBlocks/CarV2/CarV2/Resource/inception_v4_299_quant.tflite");
    //std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/user/CodeBlocks/CarV2/CarV2/Resource/mobilenet_v1_1.0_224_quant.tflite");
    //std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/user/CodeBlocks/CarV3/CarV3/Resource/Model_Traffic_Signs_TFLite.tflite");
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile("/home/user/CodeBlocks/CarV3/CarV3/Resource/Model_Traffic_Sign_TFLite3.tflite");

    // Build the interpreter
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);

    interpreter->AllocateTensors();
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(4);      //quad core

    // Get input dimension from the input tensor metadata
    // Assuming one input only
    int batch_size;
    int model_width;
    int model_height;
    int model_channels;
    int In = interpreter->inputs()[0];

    batch_size     = interpreter->tensor(In)->dims->data[0];
    model_height   = interpreter->tensor(In)->dims->data[1];
    model_width    = interpreter->tensor(In)->dims->data[2];
    model_channels = interpreter->tensor(In)->dims->data[3];

	// Get the model labels
	bool result = getFileContent("/home/user/CodeBlocks/CarV3/CarV3/Resource/labels.txt");
	if(!result)
	{
        cout << "loading labels failed";
        exit(-1);
	}

    Mat frame = preprocess_image(img, model_width, model_height);

    if (frame.empty()) {
        cerr << "Can not load picture!" << endl;
        exit(-1);
    }
    cout << " " << endl;
    // copy image to input as input tensor
    //cv::resize(frame, image, Size(model_width,model_height),INTER_NEAREST);
    //memcpy(interpreter->typed_input_tensor<uchar>(0), image.data, image.total() * image.elemSize());

    //cout << "tensors size: " << interpreter->tensors_size() << "\n";
    //cout << "nodes size: " << interpreter->nodes_size() << "\n";
    //cout << "inputs: " << interpreter->inputs().size() << "\n";
    //cout << "outputs: " << interpreter->outputs().size() << "\n";

    // Run model prediction with a timer
    chrono::steady_clock::time_point Tbegin = chrono::steady_clock::now();
    interpreter->Invoke(); 
    chrono::steady_clock::time_point Tend = chrono::steady_clock::now();

    const float threshold = 0.001f;
    std::vector<std::pair<float, int>> top_results;
    int output = interpreter->outputs()[0];
    TfLiteIntArray* output_dims = interpreter->tensor(output)->dims;

    // assume output dims to be something like (1, 1, ... ,size)
    auto output_size = output_dims->data[output_dims->size - 1];
    cout << "output_size: " << output_size <<"\n";

    switch (interpreter->tensor(output)->type) {
        case kTfLiteFloat32:
            tflite::label_image::get_top_n<float>(interpreter->typed_output_tensor<float>(0), output_size,
                                                    5, threshold, &top_results, kTfLiteFloat32);
        break;
        case kTfLiteUInt8:
            tflite::label_image::get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0), output_size,
                                                    5, threshold, &top_results, kTfLiteUInt8);
        break;
        default:
            cerr << "cannot handle output type " << interpreter->tensor(output)->type << endl;
            exit(-1);
    }

    for (const auto& result : top_results) {

        const float confidence = result.first;
        const int index = result.second;
        label = this->Labels[index];

        cout << confidence << " : " << this->Labels[index] << "\n";
        //label = "Stop";

        //cout << "index: " << index << endl;
        //cout << "Label: " << label << endl;
        break;
    }

    //calculate time
    int prediction_time = chrono::duration_cast <chrono::milliseconds> (Tend - Tbegin).count();
    cout << "Prediction time: " << prediction_time << " mSec" << endl;

	return label;
}

// Function preprocess the image to fit model input
Mat AI::preprocess_image(Mat& img, int model_width, int model_height) {

    // Preprocess the img before prediction
    // Model training use to greyscale, equalize and normalize with divide by 255

    // img = imread("/home/user/CodeBlocks/CarV3/CarV3/Resource/tfS.jpg");

    std::cout << "            " << std::endl;
    //std::cout << "img: " << img.size << std::endl;

    // Sharp the img (optional)
    Mat sharp;
    Mat sharpening_kernel = (Mat_<double>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);
    filter2D(img, sharp, -1, sharpening_kernel);

    imshow("img", img);
    imshow("sharp", sharp);

    cvtColor(img, img, COLOR_BGR2GRAY);
    equalizeHist(img, img);
    img.convertTo(img, CV_32FC1, 1.0 / 255.0);
    cv::resize(img, img, Size(model_width, model_height), INTER_NEAREST);

    cout << "Image type " << img.type() << endl;

    // Change image to tensor model input 
    int size_2[4] = {1, img.rows, img.cols, img.channels() };
    cv::Mat image(4, size_2, img.type(), img.data);
    std::cout << "image: " << image.size << std::endl;

    std::cout << "image.dims = " << image.dims << " image.size = [";
    for (int i = 0; i < image.dims; ++i) {
    	if (i) std::cout << " X ";
    	std::cout << image.size[i];
    }
    std::cout << "] image.channels = " << image.channels() << std::endl;

    return img;
}

bool AI::getFileContent(std::string fileName){
	// Open the File
	std::ifstream in(fileName.c_str());
	// Check if object is valid
	if(!in.is_open()) return false;

	std::string str;
	// Read the next line from File untill it reaches the end.
	while (std::getline(in, str))
	{
		// Line contains string of length > 0 then save it in vector
		if(str.size()>0) Labels.push_back(str);
	}
	// Close The File
	in.close();
	return true;
}

// Function drawing rectangle on the image
void AI::draw_rectangle(Mat& image, Rect boundRect, string label) {

    //cout << "X: " << boundRect.x << endl;
    //cout << "Y: " << boundRect.y << endl;
    //cout << " " << endl;

    label.erase(std::remove(label.begin(), label.end(), '\r'), label.end());

	rectangle(image, boundRect.tl(), boundRect.br(), Scalar(0, 200, 0), 3);
	putText(image, label, { boundRect.x - 1, boundRect.y - 20}, FONT_HERSHEY_COMPLEX, 0.7, Scalar(0, 200, 0), 2);
//	putText(image, label, { boundRect.x - 100, boundRect.y - 15 }, FONT_HERSHEY_COMPLEX, 0.7, Scalar(175, 50, 175), 2);
//	putText(image, label, { int(image.cols / 2) - 120, 30 }, FONT_HERSHEY_COMPLEX, 0.7, Scalar(175, 50, 175), 2);
}

// Function find the car direction acoording to the curve direction
float AI::aim_car_direction(int curve_direction) {

	float sensitivity = 1.3f;

	float max_speed = float(float(this->speed) / 100.f);

	float average_curve = this->get_average_direction(curve_direction);

	// Limit the speed
	if (max_speed < average_curve) {
		average_curve = max_speed;
	}
	if (average_curve < -max_speed) {
		average_curve = -max_speed;
	}

	if (0 < average_curve) {
		sensitivity = 1.9f;
		if (average_curve < 0.05) {
			average_curve = 0;
		}
	}
	else {
		if (-0.08 < average_curve) {
			average_curve = 0;
		}
	}
	//float turn = round((-average_curve * sensitivity) * 10000) / 10000;
	float turn = (average_curve * sensitivity);

	return turn;
}

// Function calculate the average of curve direction
float AI::get_average_direction(int curve_direction) {

	float sum = 0;

	int average_number = 10;

	float average_curve = 0;

	this->curve_list[this->curve_counter] = curve_direction;

	this->curve_counter += 1;

	// Init the counter in case is large the length array
	if (average_number <= this->curve_counter) {
		this->curve_counter = 0;
	}

	// Sum the elements to find average
	for (int i = 0; i < average_number; i++) {
		sum = sum + curve_list[i];
	}

	// Convert average in decimal
	average_curve = (sum / average_number) / 10;

	// Normalize the average value
	if (1 < average_curve) {
		average_curve = 1;
	}
	if (average_curve < -1) {
		average_curve = -1;
	}

	return average_curve;
}

void AI::init_detection(){

    this->onTracking = false;
	this->trafficSignDetected = false;
}
