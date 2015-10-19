Homework 1.b
Ben Matern


I use runProgram.sh to compile and run my program.  It's using g++ on a macbook.  
Sorry, I didn't get a chance to fire up an ubuntu VM to give it a test.  I can get that for hte next homework.
If it doesn't run by default, i'd be interested to know what I did wrong.

Input file name can be passed in as a parameter, like this:
./BenMaternHwk1 bens_image.txt

If there is no parameter passed, the program defaults to run some demos
, which I use to demonstrate a variety of viewing conditions.
./BenMaternHwk1 

I guess I'll include my brief writeup here.

They demos currently will spit out 5 images.  
Each one contains 9 rendered images stitched together.
I have here a brief description of how they demonstrate the functionality of my raytracer.
Use the images as references.

ShiftCameraDemo.ppm
	-This demo shifts the camera along the x and y axes. 
	-This demonstrates what happens when the camera moves
ShiftLightSourceDemo.ppm
	-This demo Shifts the light source along the y axis.
	-The movement of the specular light is most obvious..
ShiftUpVectorDemo.ppm
	-This demo changes the up vector, with comical results.
	-I only really rotated the z value, other up vectors are possible.
AdjustSurfaceDemo.ppm
	-This demo changes the surface properties of the spheres.  
	-Ka is decreased in red spheres.  Kd is decreased in the blue spheres.  ks is decreased in the green spheres.
	-This demonstrates the differences in surface constants.
ShiftFOV.ppm
	-The FOV is altered, from 20 to 173.
	-When FOV is small, the image is "zoomed in" on what is directly in front of us.








