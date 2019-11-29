// Author: Ram Srivatsav Ghorakavi
// Company: HullBot pty ltd.
// License: 
// For execution: gcc ./path_to_file -o out -lrealsense2 -lGL -lglut -lGLU (in VS)
//                ./out

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

/* Include the required librealsense C header files */
#include <librealsense2/rs.h>
#include <librealsense2/h/rs_pipeline.h>

/* Function calls to librealsense may raise errors of type rs_error*/
void check_error(rs2_error* e)
{
    if (e)
    {
        printf("rs_error was raised when calling %s(%s):\n", rs2_get_failed_function(e), rs2_get_failed_args(e));
        printf("    %s\n", rs2_get_error_message(e));
        exit(EXIT_FAILURE);
    }
}

void print_device_info(rs2_device* dev)
{
    rs2_error* e = 0;
    printf("\nUsing device 0, an %s\n", rs2_get_device_info(dev, RS2_CAMERA_INFO_NAME, &e));
    check_error(e);
    printf("    Serial number: %s\n", rs2_get_device_info(dev, RS2_CAMERA_INFO_SERIAL_NUMBER, &e));
    check_error(e);
    printf("    Firmware version: %s\n\n", rs2_get_device_info(dev, RS2_CAMERA_INFO_FIRMWARE_VERSION, &e));
    check_error(e);
}

int main (){
    printf("\n             Welcome!\n\n");
	rs2_error* e = 0;

    // Create a context object. This object owns the handles to all connected realsense devices.
    // The returned object should be released with rs2_delete_context(...)
    rs2_context* ctx = rs2_create_context(RS2_API_VERSION, &e);
    check_error(e);

    /* Get a list of all the connected devices. */
    // The returned object should be released with rs2_delete_device_list(...)
    rs2_device_list* device_list = rs2_query_devices(ctx, &e);
    check_error(e);
    int dev_count = rs2_get_device_count(device_list, &e);
    check_error(e);
    printf("There are %d connected RealSense devices.\n", dev_count);

    // Get the first connected device
    // The returned object should be released with rs2_delete_device(...)
    rs2_device* dev = rs2_create_device(device_list, 0, &e);
    check_error(e);

    print_device_info(dev);

    // Create a pipeline to configure, start and stop camera streaming
    // The returned object should be released with rs2_delete_pipeline(...)
    rs2_pipeline* pipeline =  rs2_create_pipeline(ctx, &e);
    check_error(e);

    // Create a config instance, used to specify hardware configuration
    // The retunred object should be released with rs2_delete_config(...)
    rs2_config* config = rs2_create_config(&e);
    check_error(e);
    
    // Start the pipeline streaming
    // The retunred object should be released with rs2_delete_pipeline_profile(...)
    rs2_pipeline_profile* pipeline_profile = rs2_pipeline_start_with_config(pipeline, config, &e);
    check_error(e);

    rs2_sensor_list* sensor_list = rs2_query_sensors(dev, &e);
    check_error(e);

    int num_of_sensors = rs2_get_sensors_count(sensor_list, &e);
    check_error(e);
    printf("number of sensors: %d\n", num_of_sensors);
    rs2_sensor* sensor = rs2_create_sensor(sensor_list, 0, &e);
    check_error(e);

    rs2_stream_profile_list* stream_profile_list = rs2_pipeline_profile_get_streams(pipeline_profile, &e);
    check_error(e);

    rs2_stream_profile* stream_profile = (rs2_stream_profile*)rs2_get_stream_profile(stream_profile_list, 0, &e);
    check_error(e);

    rs2_intrinsics intr;
    rs2_get_video_stream_intrinsics(stream_profile, &intr, &e);
    printf("\nInitial Intrinsics:\n");
    printf("width: %d\n", intr.width);
    printf("height: %d\n", intr.height);
    printf("PPX: %lf\n", intr.ppx);
    printf("PPY: %lf\n", intr.ppy);
    printf("Fx: %lf\n", intr.fx);
    printf("Fy: %lf\n", intr.fy);
    printf("model: %s\n", rs2_distortion_to_string(intr.model));
    printf("Distortion_coeffs: %lf, %lf, %lf, %lf, %lf\n", intr.coeffs[0], intr.coeffs[1], intr.coeffs[2], intr.coeffs[3], intr.coeffs[4]);
    
    ////////////////////////////////////////
    // allocation of new intrinsics values//
    ////////////////////////////////////////
    // this is initially configured to set the default T265 parameter values. change according to your need.
    // The process can be automated based on the calibration tool.

    intr.width = intr.width; // change to required parameter value!
    intr.height = intr.height; // change to required parameter value!
    intr.ppx = intr.ppx; // change to required parameter value!
    intr.ppy = intr.ppy; // change to required parameter value!
    intr.fx = intr.fx; // change to required parameter value!
    intr.fy = intr.fy; // change to required parameter value!
    intr.model = intr.model; // change to required parameter value!
    intr.coeffs[0] = intr.coeffs[0]; // change to required parameter value!
    intr.coeffs[1] = intr.coeffs[1]; // change to required parameter value!
    intr.coeffs[2] = intr.coeffs[2]; // change to required parameter value!
    intr.coeffs[3] = intr.coeffs[3]; // change to required parameter value!
    intr.coeffs[4] = intr.coeffs[4]; // change to required parameter value!

    ///////////////////////
    // end of allocation //
    ///////////////////////

    // setting new intrinsics.
    rs2_set_intrinsics (sensor, stream_profile, &intr, &e);
    check_error(e);

    // validation of the new intrinsics.
    rs2_get_video_stream_intrinsics(stream_profile, &intr, &e);
    printf("\nAfter modification:\n");
    printf("width: %d\n", intr.width);
    printf("height: %d\n", intr.height);
    printf("PPX: %lf\n", intr.ppx);
    printf("PPY: %lf\n", intr.ppy);
    printf("Fx: %lf\n", intr.fx);
    printf("Fy: %lf\n", intr.fy);
    printf("model: %s\n", rs2_distortion_to_string(intr.model));
    printf("Distortion_coeffs: %lf, %lf, %lf, %lf, %lf\n", intr.coeffs[0], intr.coeffs[1], intr.coeffs[2], intr.coeffs[3], intr.coeffs[4]);

    // Stop the pipeline streaming
    rs2_pipeline_stop(pipeline, &e);
    check_error(e);

    // Release resources
    rs2_delete_sensor(sensor);
    rs2_delete_sensor_list(sensor_list);
    rs2_delete_pipeline_profile(pipeline_profile);
    rs2_delete_config(config);
    rs2_delete_pipeline(pipeline);
    rs2_delete_device(dev);
    rs2_delete_device_list(device_list);
    rs2_delete_context(ctx);
    return EXIT_SUCCESS;
}