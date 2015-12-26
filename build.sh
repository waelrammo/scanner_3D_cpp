g++ test.cpp -I/usr/local/include/opencv -L/usr/local/include/opencv -lopencv_core -o hello_world
g++ frangi_filter.cpp extract_line.cpp -I. -I/usr/local/include/opencv -L/usr/local/include/opencv -g -lopencv_highgui -lopencv_core -o extract_line
g++ hough.cpp -I. -I/usr/local/include/opencv -L/usr/local/include/opencv -g -lopencv_highgui -lopencv_core -lopencv_imgproc -o hough

