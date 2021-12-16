/* SUDOKU SOLVER:                                                                  ~Nitin Rohit
                  
This is a code which I have written from scratch without any internet aid which can be used to solve 
sudoku of any valid dimension, which we can change using the variables which I will explain. In this 
code I have used basic user defined functions. I'll will explain how the code works at each required 
part so that you don't have a hard time understading a simple code. */

#include<iostream>
using namespace std;

// r can be any integer and s should be the square of it, where s is the dimension of sudoku.
const int r=3,s=9;

// This function gets the value of sudoku elements from user using a 2-d loop.
void getSudoku(int (*grid)[s])
     {
         cout<<"Enter the values of sudoku:\n";
         for(int i=0;i<s;i++)
         for(int j=0;j<s;j++) 
         cin>>grid[i][j];
     }    

/* Value-array: I have defined a 3-d array which stores 1s and 0s for each element of the sudoku grid.
It stores 1 if the a number have a possibility to occur at that box if not possible then 0 is assigned. */

/* This function fills the value-array for the existing elements with 1 for the number present and 0s 
for rest of the numbers. */
void fill(int (*value)[s][s], int (*grid)[s],int i, int j)
     {
         for(int k=0;k<s;k++)
         value[i][j][k]=0;
         value[i][j][grid[i][j]-1]=1;
     }

// Intialising the value-array with ones as all numbers can be a potential candidate for an empty sudoku.
void intialisation1(int (*value)[s][s])
     {
         for(int i=0;i<s;i++)
         for(int j=0;j<s;j++)
         for(int k=0;k<s;k++)
         value[i][j][k]=1;
     }

// Intialising the sum array with zeros as sum is intially zero for all elements.
void intialisation2(int (*sum)[s])     
     {
         for(int i=0;i<s;i++)
         for(int j=0;j<s;j++)
             sum[i][j]=0;
     }

// This fucntion returns the value of numbers of boxs left unsolved so that loop can run until then.
int endfun(int (*grid)[s])
     {
         int end=0;
         for(int i=0;i<s;i++)
         for(int j=0;j<s;j++)
            if(!grid[i][j])
               end=end+1;

          return end;
     }     

/* This function assigns the value of value-array using the sudoku logic,i.e, all elements in row, 
column and box should be unique simulataneously */     
void getValue(int (*value)[s][s],int (*grid)[s])
     {
         for(int i=0;i<s;i++)
         for(int j=0;j<s;j++)
         if(grid[i][j])
             fill(value,grid,i,j);
         else 
         {
             for(int m=0;m<s;m++)
                 if(grid[m][j])
                     value[i][j][grid[m][j]-1]=0;
             for(int n=0;n<s;n++)
                 if(grid[i][n])
                     value[i][j][grid[i][n]-1]=0;   
             for(int x=(i/r)*r;x<(i/r)*r+r;x++)
             for(int y=(j/r)*r;y<(j/r)*r+r;y++)      
                  if(grid[x][y])
                      value[i][j][grid[x][y]-1]=0;   
         }      

     }

/* As the name suggests it gives the sum of all elements along the 3rd dimension of the value-array,
we will use this  array values to find weather a element can occur there with assurity. The value-array 
gives the numbers which can occur or not, so a number can occur there if no other number can't occur 
there apart from itself. Which means that if the sum is one then it has a unique solution.*/     
void getSum(int (*value)[s][s],int (*sum)[s])
     {
         intialisation2(sum);
         for(int i=0;i<s;i++)
         for(int j=0;j<s;j++)
         for(int k=0;k<s;k++)
             sum[i][j]=value[i][j][k]+sum[i][j];

     }

// This function gives the sum of particular row of all the elements value-array in 3rd dimension     
void getHsum(int *hsum, int (*value)[s][s],int i)
     {
         for(int k=0;k<s;k++)
             hsum[k]=0;

         for(int k=0;k<s;k++)
         for(int l=0;l<s;l++) 
             hsum[k]=hsum[k]+value[i][l][k];   
     }

// This function gives the sum of particular column of all the elements value-array in 3rd dimension
void getVsum(int *vsum, int (*value)[s][s],int j)
     {
         for(int k=0;k<s;k++)
             vsum[k]=0;

         for(int k=0;k<s;k++)
         for(int l=0;l<s;l++) 
             vsum[k]=vsum[k]+value[l][j][k];
     }

// This function gives the sum of particular box of all the elements value-array in 3rd dimension
void getBsum(int *bsum, int (*value)[s][s],int i,int j)
     {
         for(int k=0;k<s;k++)
             bsum[k]=0;

         for(int k=0;k<s;k++)
         for(int l=0;l<r;l++) 
         for(int m=0;m<r;m++)
             bsum[k]=bsum[k]+value[i+l][j+m][k];
     }

/*  So this fucntion checks if the square has a unique solution according to the usual sudoku rule,i.e,
if only a single element is being common(row, column and box) to that square then that is the solution. 
It first uses the sum array to check if there exists a unique solution(sum value of that box will be one 
as only it is unique solution), if yes then finds that number and updates the sudoku array along with 
the value-array.  */
void intersection(int (*value)[s][s], int (*grid)[s],int (*sum)[s])
     {
         getSum(value,sum);
         
         for(int i=0;i<s;i++)
         for(int j=0;j<s;j++)
            if(sum[i][j]==1)
               for(int k=0;k<s;k++)
                   if(value[i][j][k])
                      {
                          grid[i][j]=k+1;
                          getValue(value,grid);
                          break;
                      }
     }

/* As the name suggests the function uses the concept of complementary. This function checks if a number 
can't occur in other squares except for one square, if so the number has to come in that square. This 
has to be checked along row, column and box. So we use the sum array along the required direction to check 
if that unique solution exists by equating it to one as done in previous funciont. */ 
  void complement(int (*value)[s][s], int (*grid)[s],int *hsum,int *vsum,int *bsum)
      {
          for(int i=0;i<s;i++)
              {
                  getHsum(hsum,value,i);
                  for(int x=0;x<s;x++)
                      if(hsum[x]==1)
                         for(int y=0;y<s;y++)
                             if(value[i][y][x]==1)
                                {
                                    grid[i][y]=x+1;
                                    getValue(value,grid);
                                }
              }
          
          for(int j=0;j<s;j++)
              {
                  getVsum(vsum,value,j);
                  for(int x=0;x<s;x++)
                      if(vsum[x]==1)
                         for(int y=0;y<s;y++)
                             if(value[y][j][x]==1)
                                {
                                    grid[y][j]=x+1;
                                    getValue(value,grid);
                                } 
              }    

          for(int i=0;i<s;i=i+r)
          for(int j=0;j<s;j=j+r)    
              {
                  getBsum(bsum,value,i,j);
                  for(int x=0;x<s;x++)
                      if(bsum[x]==1)
                         for(int y=0;y<r;y++)
                         for(int z=0;z<r;z++)
                             if(value[y+i][z+j][x]==1)
                                {
                                    grid[y+i][z+j]=x+1;
                                    getValue(value,grid);
                                } 
              }
      }

  // This function can display a 2-d array of dimension same as of sudoku    
  void display(int (*grid)[s])
     {
         for(int i=0;i<s;i++)
         {
            for(int j=0;j<s;j++)
            cout<<grid[i][j]<<" ";
            cout<<"\n";
         }
     }
  

// Now we just have to call the functions in main function accordingly and run the code
  int main()
     {
         int value[s][s][s],grid[s][s],sum[s][s],hsum[s],vsum[s],bsum[s],i,j,k,end;
         getSudoku(grid);
         cout<<"\n";
         intialisation1(value);
         end=endfun(grid);
         getValue(value,grid);
         
         while(end) 
         {
             intersection(value,grid,sum);
             complement(value,grid,hsum,vsum,bsum);
             end=endfun(grid);
         }    

         display(grid);
         return 0; 
     }   

// THANK YO