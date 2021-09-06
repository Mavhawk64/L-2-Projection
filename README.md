# L-2-Projection
This is a C program for the L^2 Projection from Finite Element Methods.

You can run this just like any other c program, and this one can accept arguments.

The arguments must be a well-formatted expression of a function with x as the independent value.
For example:

> ./a.out x ^ 2 + 3.14

It will create 4 text documents with the 4 outputs.

What I would recommend doing, is copy all of the line equations from the 'eqs.txt' file and paste them into desmos - it comes up with a really cool graph of all of the rays (assuming x ^ n, where n > 1).

**Edit**

It turns out that the c program does not create PhU to create a piecewise linear interpolation. I am going to temporarily drop the C program and move it to another language (likely JS, Java, or Python because those are the easiest to run).
