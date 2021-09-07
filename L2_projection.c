#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
double** assembleMassMatrix(int, double*);
double* assembleLoadVectorWithFunction(int, double*, int, const char**);
double* assembleLoadVectorWithValues(int, double*,double*);
double getFunctionVal(int, const char**, double); //Prototype --only works with simple equations and DOES NOT NECESSARILY FOLLOW ORDER OF OPERATIONS
double** rref(int, double**, double*);
void multiplyRowBy(int,double*,double);
double** ref(int, double**);
void subtractRowByFactor(int, double*, double*, double);
int main(int argv, const char* argc[]) {

//Input

  printf("Enter subintervals (n): ");
    int nInput;
    scanf("%d", &nInput);
    nInput++;
    double* xInput = (double*)malloc(nInput*sizeof(double));
    printf("Would you like evenly spaced intervals or manual intervals?\n1. Evenly Spaced Intervals\n2. Manual Intervals\n");
    int choice1;
    scanf("%d", &choice1);
    if(choice1 == 2) {
      printf("** Manual Intervals **\nNOTE: Each value must be larger than the previous!!!\n");
      int ind;
      printf("Are you zero-indexed or one-indexed? (0 / 1): ");
      scanf("%d", &ind);
      for(int i = 0; i < nInput; i++) {
        printf("Enter x[%d]: ", i+ind);
        scanf("%lf", &xInput[i]);
      }
    } else {
      printf("** Evenly Spaced Intervals **\nNOTE: Starting index is smaller than ending index (a < b)!!!\nEnter starting index (a): ");
      double a;
      scanf("%lf", &a);
      printf("Enter ending index (b): ");
      double b;
      scanf("%lf", &b);
      if(a>b) {
        double temp = a;
        a = b;
        b = temp;
      }
      double h = (b - a) / (nInput-1);
      printf("x = \n");
      for(int i = 0; i < nInput; i++) {
        xInput[i] = a + h * i;
        printf("| %.2lf |\n", xInput[i]);
      }
    }
    double* uInput = (double*)malloc(nInput * sizeof(double));
    if(argv == 1) {
      printf("I noticed that you didn't enter in a function. You can do that when running the file:\n>./a.out x ^ 2 + 3\n");
      printf("Enter in the function values at each value:\n");
      for(int i = 0; i < nInput; i++) {
        printf("u(%.2lf) = ", xInput[i]);
        scanf("%lf", &uInput[i]);
      }
    }

   //Grab data

    double** mOutput = assembleMassMatrix(nInput, xInput);
    double* bOutput =
        argv == 1 ? assembleLoadVectorWithValues(nInput, xInput, uInput)
                  : assembleLoadVectorWithFunction(nInput, xInput, argv, argc);
    double** augmentedMatrix = rref(nInput, mOutput, bOutput);

    // Output
    printf(
        "Would you like all outputs or just coordinates?\n1. All\n2. Just "
        "Coordinates\n");
    int choice2;
    scanf("%d", &choice2);
    if(choice2 == 1) {
    printf("\nM = \n");
    for(int i = 0; i < nInput; i++) {
        printf("| ");
        for(int j = 0; j < nInput-1; j++) {
            printf("%.2lf  ", mOutput[i][j]);
        }
        printf("%.2lf |\n", mOutput[i][nInput-1]);
    }
    printf("\nb = \n");
    for(int i = 0; i < nInput; i++) {
        printf("| %.2lf |\n", bOutput[i]);
    }

    printf("\nM|b:\n");
    for(int i = 0; i < nInput; i++) {
        printf("| ");
        for(int j = 0; j < nInput-1; j++) {
            printf("%.2lf  ", mOutput[i][j]);
        }
        printf("%.2lf | %.2lf |\n", mOutput[i][nInput-1], bOutput[i]);
    }
    
    printf("\nrref(M|b) = \n");
    for(int i = 0; i < nInput; i++) {
      printf("| ");
      for(int j = 0; j < nInput; j++) {
        printf("%.0lf ", augmentedMatrix[i][j]);
      }
      printf("| %.2lf |\n", augmentedMatrix[i][nInput]);
    }
    printf("C = \n");
    for(int i = 0; i < nInput; i++) {
      printf("| %.2lf |\n", augmentedMatrix[i][nInput]);
    }
}
printf("\nHow accurate would you like your decimals (amt. decimal places)? (Ex: 2 gives 3.14 , 5 gives 3.14159)\n");
int choice3;
scanf("%d", &choice3);
    printf("\nCoordinates: \n");
    for(int i = 0; i < nInput; i++) {
      printf("(%.*lf, %.*lf)\n", choice3, xInput[i], choice3, augmentedMatrix[i][nInput]);
    }
  
  printf("\nWould you like the equations of the lines? (y/n)\n");
  char choice4;
  scanf("\n%c", &choice4);
  if(choice4 == 'n') return 0;
  for(int i = 1; i < nInput; i++) {
    double x1 = xInput[i-1];
    double x2 = xInput[i];
    double y1 = augmentedMatrix[i-1][nInput];
    double y2 = augmentedMatrix[i][nInput];
    double slope = (y2 - y1) / (x2 - x1);
    printf("y = %.*lf(x - %.*lf) + %.*lf\n", choice3, slope, choice3, x1, choice3, y1);
  }
  if(choice4 == 'y') {
  printf("Here is the piecewise interpolation:\n");
  double x1 = xInput[0];
  double x2 = xInput[1];
  double y1 = augmentedMatrix[0][nInput];
  double y2 = augmentedMatrix[1][nInput];
  double slope = (y2 - y1) / (x2 - x1);
  printf("y = {%.*lf<=x<=%.*lf:%.*lf(x-%.*lf)+%.*lf", choice3, x1, choice3, x2, choice3, slope, choice3, x1, choice3, y1);
  for(int i = 2; i < nInput; i++) {
    x1 = xInput[i-1];
    x2 = xInput[i];
    y1 = augmentedMatrix[i - 1][nInput];
    y2 = augmentedMatrix[i][nInput];
    slope = (y2 - y1) / (x2 - x1);
    printf(",%.*lf<=x<=%.*lf:%.*lf(x-%.*lf)+%.*lf", choice3, x1, choice3, x2, choice3, slope, choice3, x1, choice3, y1);
  }
  printf("}\n");
  }


// FILE OUTPUT




  printf("Would you like to save the outputs to files? (y/n)\n");
  char choice5;
  scanf("\n%c", &choice5);
  if(choice5 == 'n') return 0;

  FILE *f = fopen("fulloutput.txt", "w");
  FILE *xmbrc = fopen("xmbrc.txt", "w");
  FILE *coord = fopen("coord.txt", "w");
  FILE *eqs = fopen("eqs.txt", "w");
  if(f == NULL || xmbrc == NULL || coord == NULL || eqs == NULL) {
    printf("Files didn't load correctly!\n");
    exit(1);
  }
  fprintf(f, "x = \n");
  fprintf(xmbrc, "x = \n");
  for (int i = 0; i < nInput; i++) {
    fprintf(f, "| %.2lf |\n", xInput[i]);
    fprintf(xmbrc, "| %.2lf |\n", xInput[i]);
  }
  fprintf(f,"%s","\nM = \n");
  fprintf(xmbrc, "\nM = \n");
  for (int i = 0; i < nInput; i++) {
    fprintf(f, "%s", "| ");
    fprintf(xmbrc, "%s", "| ");
    for (int j = 0; j < nInput - 1; j++) {
      fprintf(f, "%.2lf  ", mOutput[i][j]);
      fprintf(xmbrc, "%.2lf  ", mOutput[i][j]);
    }
    fprintf(f, "%.2lf |\n", mOutput[i][nInput - 1]);
    fprintf(xmbrc, "%.2lf |\n", mOutput[i][nInput - 1]);
  }
  fprintf(f,"%s","\nb = \n");
  fprintf(xmbrc, "%s", "\nb = \n");
  for (int i = 0; i < nInput; i++) {
    fprintf(f,"| %.2lf |\n", bOutput[i]);
    fprintf(xmbrc, "| %.2lf |\n", bOutput[i]);
  }

  fprintf(f,"%s","\nM|b:\n");
  fprintf(xmbrc, "%s", "\nM|b:\n");
  for (int i = 0; i < nInput; i++) {
    fprintf(f,"%s","| ");
    fprintf(xmbrc, "%s", "| ");
    for (int j = 0; j < nInput - 1; j++) {
      fprintf(f,"%.2lf  ", mOutput[i][j]);
      fprintf(xmbrc, "%.2lf  ", mOutput[i][j]);
    }
    fprintf(f,"%.2lf | %.2lf |\n", mOutput[i][nInput - 1], bOutput[i]);
    fprintf(xmbrc, "%.2lf | %.2lf |\n", mOutput[i][nInput - 1], bOutput[i]);
  }

  fprintf(f,"%s","\nrref(M|b) = \n");
  fprintf(xmbrc, "%s", "\nrref(M|b) = \n");
  for (int i = 0; i < nInput; i++) {
    fprintf(f,"%s","| ");
    fprintf(xmbrc, "%s", "| ");
    for (int j = 0; j < nInput; j++) {
      fprintf(f, "%.0lf ", augmentedMatrix[i][j]);
      fprintf(xmbrc, "%.0lf ", augmentedMatrix[i][j]);
    }
    fprintf(f, "| %.2lf |\n", augmentedMatrix[i][nInput]);
    fprintf(xmbrc, "| %.2lf |\n", augmentedMatrix[i][nInput]);
  }
  fprintf(f, "C = \n");
  fprintf(xmbrc, "C = \n");
  for (int i = 0; i < nInput; i++) {
    fprintf(f, "| %.2lf |\n", augmentedMatrix[i][nInput]);
    fprintf(xmbrc, "| %.2lf |\n", augmentedMatrix[i][nInput]);
  }

//Coordinates

  for (int i = 0; i < nInput; i++) {
    fprintf(f,"(%.*lf, %.*lf)\n", choice3, xInput[i], choice3,
           augmentedMatrix[i][nInput]);
    fprintf(coord, "(%.*lf, %.*lf)\n", choice3, xInput[i], choice3,
            augmentedMatrix[i][nInput]);
  }



//Equations

  for (int i = 1; i < nInput; i++) {
    double x1 = xInput[i - 1];
    double x2 = xInput[i];
    double y1 = augmentedMatrix[i - 1][nInput];
    double y2 = augmentedMatrix[i][nInput];
    double slope = (y2 - y1) / (x2 - x1);
    fprintf(f, "y = %.*lf(x - %.*lf) + %.*lf\n", choice3, slope, choice3, x1,
            choice3, y1);
    fprintf(eqs, "y = %.*lf(x - %.*lf) + %.*lf\n", choice3, slope, choice3, x1,
            choice3, y1);
  }

  fclose(f);
  fclose(xmbrc);
  fclose(coord);
  fclose(eqs);

  printf("\n\n\n\nSuccessfully saved output to 'fulloutput.txt', 'xmbrc.txt', 'coord.txt', and 'eqs.txt'.\n");

    return 0;
}

/*
* Function: assembleMassMatrix
* This function will assemble the mass matrix in the L^2-Projection Method.
* Parameters: int n, double* x
* n - Length and Width of Mass Matrix and Length of x
* x - Each of the subintervals between [x0,xn]
* Returns: double**
* Returns the Assembled Mass Matrix
* Note: This will only work properly when n and x are exactly correctly formatted.
*/
double** assembleMassMatrix(int n, double* x) {
    //Dynamically allocates memory for a Matrix (2-D Array) of type double.
  double **m = (double **)malloc(n * sizeof(double *));
  for (int i = 0; i < n; i++) m[i] = (double *)malloc(n * sizeof(double));
  for(int i = 0; i < n-1; i++) {
      double hi6 = (x[i+1] - x[i]) / 6.0;
    double mi[2][2] = {{hi6 + hi6, hi6}, {hi6, hi6 + hi6}};
    m[i][i] += mi[0][0];
    m[i][i+1] += mi[0][1];
    m[i+1][i] += mi[1][0];
    m[i+1][i+1] += mi[1][1];
  }
  return m;
}
/*
 * Function: assembleLoadVectorWithFunction
 * This function will assemble the load vector in the L^2-Projection Method.
 * Parameters: int n, double* x, char* u[]
 * n - Length of Load Vector
 * x - Each of the subintervals between [x0, xn]
 * u - The function as a String (x^2 + 2)
 * Returns: double*
 * Returns the Assembled Load Vector
 * Note: This will only work properly when n and u are exactly correctly
 * formatted.
 * Also note that this is under development and will only work with certain functions (maybe).
 */
double* assembleLoadVectorWithFunction(int n, double* x, int argv, const char** u) {
    double* b = (double*)malloc(n * sizeof(double));
    for(int i = 0; i < n-1; i++) {
        double hi2 = (x[i+1] - x[i]) / 2.0;
        double bi[2] = {hi2 * getFunctionVal(argv, u, x[i]), hi2 * getFunctionVal(argv, u, x[i+1])};
        b[i] += bi[0];
        b[i+1] += bi[1];
    }
    return b;
}
/*
 * Function: assembleLoadVectorWithValues
 * This function will assemble the load vector in the L^2-Projection Method.
 * Parameters: int n, double* x, double* u
 * n - Length of Load Vector
 * x - Each of the subintervals between [x0, xn]
 * u - The values of the function at each point x[i]
 * Returns: double*
 * Returns the Assembled Load Vector
 * Note: This will only work properly when n and u are exactly correctly
 * formatted.
 * Also note that this is under development and will only work with certain
 * functions (maybe).
 */
double* assembleLoadVectorWithValues(int n, double* x, double* u) {
  double* b = (double*)malloc(n * sizeof(double));
  for (int i = 0; i < n - 1; i++) {
    double hi2 = (x[i + 1] - x[i]) / 2.0;
    double bi[2] = {hi2 * u[i],
                    hi2 * u[i+1]};
    b[i] += bi[0];
    b[i + 1] += bi[1];
  }
  return b;
}
/**
 * Function: getFunctionVal
 * This returns the value of a basic function.
 * Parameters: char* u[], double xi
 * u - The expression to evaluate
 * xi - The value at which to evaluate the function
 * Returns: double
 * Returns the value of a basic function.
 * Note: This will not (necessarily) follow the order of operations.
 * It will go from left to right. 
 **/
double getFunctionVal(int argv, const char** u, double xi) {
    double ret = 0;
    int hasOperator = -1;
    double parsedNum = -1;
    for(int i = 1; i < argv; i++) {
          switch (u[i][0]) {
            case 'x':
              switch (hasOperator) {
                case 1:
                  ret += xi;
                  break;
                case 2:
                  ret -= xi;
                  break;
                case 3:
                  ret *= xi;
                  break;
                case 4:
                  ret /= xi;
                  break;
                case 5:
                  if (xi == 0) ret = 1;
                  ret = pow(ret, xi);
                  break;
                default:
                  ret += xi;
                  break;
              }
              hasOperator = -1;
              break;
            case '+':
              hasOperator = 1;
              break;
            case '-':
              hasOperator = 2;
              break;
            case '*':
              hasOperator = 3;
              break;
            case '/':
              hasOperator = 4;
              break;
            case '^':
              hasOperator = 5;
              break;
            default:
            //is number
            parsedNum = atof(u[i]); //Ability to read doubles.
            switch (hasOperator) {
              case 1:
                ret += parsedNum;
                break;
              case 2:
                ret -= parsedNum;
                break;
              case 3:
                ret *= parsedNum;
                break;
              case 4:
                ret /= parsedNum;
                break;
              case 5:
                ret = pow(ret, parsedNum);
                break;
              default:
                ret += parsedNum;
                break;
            }
            hasOperator = -1;
              break;
          }
    }
    return ret;
}
/**
 * Let M = [[a,b,c,d,e],[f,g,h,i,j],[k,l,m,n,o],[p,q,r,s,t]]
 *
 * Call REF Function to get:
 * 
 * Current State (REF)
 * | 1 b c d | e |
 * | 0 1 h i | j |
 * | 0 0 1 n | o |
 * | 0 0 0 1 | t |
 * 
 * Now time to RREF from REF
 * 
 * R2 - n*R3
 * | 0 0 1 0 | o |
 * R1 - i*R3
 * | 0 1 h 0 | j |
 * R1 - h*R2
 * | 0 1 0 0 | j |
 * R0 - d*R3
 * | 1 b c 0 | e |
 * R0 - c*R2
 * | 1 b 0 0 | e |
 * R0 - b*R1
 * | 1 0 0 0 | e |
 *
 * Current State (RREF)
 * | 1 0 0 0 | e |
 * | 0 1 0 0 | j |
 * | 0 0 1 0 | o |
 * | 0 0 0 1 | t |
 **/
double** rref(int n, double** m, double* b) {
    //Define mass
  double** mass = (double**)malloc(n * sizeof(double*));
  for (int i = 0; i < n; i++) mass[i] = (double*)malloc((n+1) * sizeof(double));
  //Load Mass
  for(int i = 0; i < n; i++) {
      for(int j = 0; j < n; j++) {
          mass[i][j] = m[i][j];
      }
      mass[i][n] = b[i];
  }
  ref(n, mass); //Now we have a matrix of form [[1,b,c,d,e],[0,1,h,i,j],[0,0,1,n,o],[0,0,0,1,t]] -- size of example doesn't matter.

  //Work our way up the matrix.
  for(int i = n-2; i >= 0; i--) {
    subtractRowByFactor(n, mass[i], mass[i+1], mass[i][i+1]);
  }
  return mass;
}
/**
 * Let M = [[a,b,c,d,e],[f,g,h,i,j],[k,l,m,n,o],[p,q,r,s,t]]
 * | a b c d | e |
 * | f g h i | j |
 * | k l m n | o |
 * | p q r s | t |
 * R0 / a
 * | 1 b c d | e |
 * R1 - f*R0
 * | 0 g h i | j |
 * R1 / g
 * | 0 1 h i | j |
 * R2 - k*R0
 * | 0 l m n | o |
 * R2 - l*R1
 * | 0 0 m n | o |
 * R2 / m
 * | 0 0 1 n | o |
 * R3 - p*R0
 * | 0 q r s | t |
 * R3 - q*R1
 * | 0 0 r s | t |
 * R3 - r*R2
 * | 0 0 0 s | t |
 * R3 / s
 * | 0 0 0 1 | t |
 *
 * Current State (REF)
 * | 1 b c d | e |
 * | 0 1 h i | j |
 * | 0 0 1 n | o |
 * | 0 0 0 1 | t |
 **/
double** ref(int n, double** aug) {
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < i; j++) {
      subtractRowByFactor(n, aug[i], aug[j], aug[i][j]);
    }
    multiplyRowBy(n + 1, aug[i], 1.0 / aug[i][i]);
  }
return aug;
}
/**
 * Function: multiplyRowBy
 * This will multiply a given row in a matrix by a factor.
 * Parameters: int len, double* r, double factor
 * len - the length of the row
 * r - the row to be multiplied by a factor
 * factor - the factor by which to multiply a row
 **/
void multiplyRowBy(int len, double* r, double factor) {
    for(int i = 0; i < len; i++) {
        r[i] *= factor;
    }
}
/**
 * Function: subtractRowByFactor
 * Takes 2 rows from a matrix and subtracts the row by the subtractand row multiplied by a factor.
 * Parameters: int n, double* row, double* sub, double factor
 * n - The width of the unaugmented matrix (n+1 = length of row)
 * row - The row to be changed
 * sub - The subtractand row
 * factor - the factor by which to multiply by a cell
 **/
void subtractRowByFactor(int n, double* row, double* sub, double factor) {
  //Create a copy of sub because we do not want it to change.
  double* copy = (double*)malloc((n+1) * sizeof(double));
  for(int i = 0; i <= n; i++) {
    copy[i] = sub[i];
  }

  for(int i = 0; i <= n; i++) {
    row[i] -= factor * copy[i];
  }
}