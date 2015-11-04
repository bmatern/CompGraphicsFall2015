Homework 1.c
Ben Matern

I use runProgram.sh to compile and run my program.  It's using g++ on a macbook.  

Input file name can be passed in as a parameter, like this:
./BenMaternHwk1 bens_image.txt
./BenMaternHwk1 bens_image2_parallel.txt
./BenMaternHwk1 bens_image2_perspective.txt

If there is no parameter passed, the program defaults to run some demos.
For this homework, parameterless will just generate the textures that I'm using.  

I only have 2 scenes prepared for this homework, but I made sure that one uses parallel projection.

PROBLEMS:  
I'm getting some artifacts in my textured spheres.  Seems to be related to light source.  I think when the light ray grazes the surface, funny things are happening, im getting white(?) pixels.  Direct light seems ok, shadows seem fine.

Sorry for the timing on this.  I tried my best, this program was a struggle for me.  Hope it's worth something, but you gotta follow the rules :D






