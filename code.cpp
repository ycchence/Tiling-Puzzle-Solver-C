
#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdlib.h>
#include<fstream>
#include <ctime>


int target[8][8];
int board[8][8] =
{
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 }
};

char ans[8][8];

int piece[100][5][5];

int pieceMin[12] =      {  1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int pieceMax[12];
int pieceMax2[12];
int pieceIdx[12] =        {  0, 4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44};
int orientMax[12] =       {  4, 4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4};

int pieceOrient[12] =           { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int pieceRow[12] =              { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int pieceCol[12] =              { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

int lastPiece = -1;
int solutionCount = 0;
bool used[60];


int pieces[60][60][60];
int small_pieces[60][8][8];
int rotat_pieces[100][8][8];
int num_pie=0;
int area[60];
int piemax [60];
int first=0;
char buffer[32];
int n_split=0;
int maxrow_split[64];
int minrow_split[64];
int maxcol_split[64];
int mincol_split[64];
int area_split[64];
int minarea_split=0;



bool comp2(int piece[100][5][5], int pi){
    for (int row = 0; row < 5; ++row)
    {
        for (int col = 0; col < 5; ++col)
        {
            if(piece[pi*4][row][col] != piece[pi*4+2][row][col])    return false;
        }
    }
    return true;
}

int split(int row, int col, int conv[60][60],int num_pie,int edgerow, int edgecol)
{
   /* if(10 > row || row > 23 || col <20|| col >24)
        return;*/
    if (conv[row][col] != -1)
    {
        pieces[num_pie][row][col] = conv[row][col];
        conv[row][col] = -1;
        area[num_pie]++;
        if(pieceMax[num_pie] == 0 )    pieceMax[num_pie] =1;
        if (0 < row && conv[row-1][col] !=-1)
        {
            
            edgerow++;
            edgecol=1;
            if(edgerow>pieceMax[num_pie])
                pieceMax[num_pie] = edgerow;
            split(row-1, col, conv, num_pie, edgerow, edgecol);
        }
        if (row < 59 && conv[row+1][col] !=-1)
        {
           edgerow++;
           edgecol=1;
           if(edgerow>pieceMax[num_pie])
                pieceMax[num_pie] = edgerow;
           split(row+1, col, conv, num_pie, edgerow, edgecol);
        }
        if (0 < col && conv[row][col-1] !=-1)
        {
           edgecol++;
           edgerow=1;

           if(edgecol>pieceMax[num_pie])
                pieceMax[num_pie] = edgecol;
            split(row, col-1, conv,num_pie, edgerow, edgecol);
        }
        if (col < 59 && conv[row][col+1] !=-1) 
        {
           edgecol++;
           edgerow=1;

           if(edgecol>pieceMax[num_pie])
                pieceMax[num_pie] = edgecol;
            split(row, col+1, conv,num_pie, edgerow, edgecol);
        }
        return 1;
    }
    return 0;
}

void matrix_rotation(int index1, int index2){
    int depth=0;
    int breadth=0;
    int temp=0;
    for(int i=0; i<8; i++){
     //   temp=0;
        for(int j=0;j<8;j++){
            if(small_pieces[index1][j][i]!=0)   
                if(j>depth)    depth = j;
        }
    }
    for(int i=0; i<8; i++){
        for(int j=0;j<8;j++){
            if(small_pieces[index1][i][j]!=0)    
                if(j>breadth)    breadth = j;
        }
    }
    for(int row=0; row<=breadth; row++){
        for(int col=0; col<=depth; col++)
            rotat_pieces[index2][col][row] = small_pieces[index1][col][row];
    }
    if(depth>breadth)
        pieceMax[index2-1]=depth+1;
    else
        pieceMax[index2-1]=breadth+1;
    //std::cout << depth << std::setw(3) <<breadth;
    for(int row=0; row<=breadth; row++){
        for(int col=0; col<=depth; col++)
            rotat_pieces[index2+1][row][col] = rotat_pieces[index2][depth-col][row];
    }
    temp = depth;
    depth = breadth;
    breadth = temp;
    for(int row=0; row<=breadth; row++){
        for(int col=0; col<=depth; col++)
            rotat_pieces[index2+2][row][col] = rotat_pieces[index2+1][depth-col][row];
    }
    temp = depth;
    depth = breadth;
    breadth = temp;
    for(int row=0; row<=breadth; row++){
        for(int col=0; col<=depth; col++)
            rotat_pieces[index2+3][row][col] = rotat_pieces[index2+2][depth-col][row];
    }
}

void matrix_size(int size){
    int minrow=60;
    int mincol=60;
    int m=0;
    int j=0;
    for(int k=0; k<size; k++){ 
        for(int row=0 ; row <60 ;row++){
            for(int col=0; col<60;col++){
                if(pieces[k][row][col]!=0){
                     if(minrow>row)
                         minrow=row;
                     if(mincol>col)
                         mincol=col;
                }
                
            }
        }
        for(int row=minrow ; row <minrow+8 ;row++){
            for(int col=mincol; col<mincol+8; col++){
               small_pieces[k][row-minrow][col-mincol]=pieces[k][row][col];
            }
        }
     //   printf(" %d \n",pieceMax[k] );
        minrow=60;
        mincol=60;

     }
        
}

void data_process()
{
    for(int i=0; i<60; i++)
        used[i]=false;
    char str[60],str2[60][60];
    for(int i=0; i<60;i++){
        str[i]=' ';
    }
    for(int i=0; i<60;i++){
        for(int j=0; j<60;j++)
            str2[i][j]=' ';
    }
    FILE* fp;
    fp = fopen("test.txt", "r");
    int jp=0;
    while (fgets(str,160, fp)) {
        for(int i=0; i<60; i++)
            str2[jp][i]=str[i];
        jp++;
    }
    int conv[60][60];
    for(int i=0; i<60; i++){
        for(int k=0 ; k<60;k++)   
            conv[i][k] = -1;
    }
    for(int i=0; i<60; i++){
        for(int j=0 ; j<60;j++)
            if(str2[i][j]!=' ' &&  str2[i][j]!=10 && str2[i][j]!=0)   
                conv[i][j] = str2[i][j];
    }
    for (int row = 0; row < 60; ++row)
    {
        for (int col = 0; col < 60; ++col)
        {
            std::cout << std::setw(3) << str2[row][col];
        }
        std::cout << '\n';
    }

    for (int row = 0; row < 60; ++row)
    {
        for (int col = 0; col < 60; ++col)
        {
            std::cout << std::setw(3) << conv[row][col];
        }
        std::cout << '\n';
    }
    fclose(fp);
    for(int row=0; row<60; row++)
        for(int col=0;col<60;col++)
            if(split( row,  col, conv, num_pie, 1, 1)==1)    num_pie++;
    
    matrix_size(num_pie);
   
     int board_index=0;
     int temp=0;
     for(int pi=0;pi<num_pie; pi++){
         if(area[pi]>temp){
             temp=area[pi];
             board_index=pi;
         }
     }
     int k1=0;
     temp=0;
     while(temp<num_pie){
         if(temp!=board_index){
             matrix_rotation(temp, k1*4);
             k1++;
         }
         temp++;
     }
    for(int i=0;i<num_pie-1;i++){
         if(i>=board_index)     area[i] = area[i+1];
    }
          
    
    for(int pi=0; pi<k1*4; pi++){
        for (int row = 0; row < 5; ++row)
        {
            for (int col = 0; col < 5; ++col)
            {
                    if(rotat_pieces[pi][row][col]!=0)    piece[pi][row][col]=rotat_pieces[pi][row][col];
                    else    piece[pi][row][col]=-1;
                    std::cout << std::setw(3) << piece[pi][row][col];
            }
            std::cout << '\n';
        }
     std::cout << '\n';
     }


     for(int pi=0; pi<k1; pi++){
        if(comp2(piece, pi) == true)     orientMax[pi] =2;  
     }
     
     for(int pi=0; pi<k1; pi++){
          
        std::cout << std::setw(3) << orientMax[pi] <<"\n" ;
     }

     for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                    if(small_pieces[board_index][row][col]!=0)    target[row][col]=small_pieces[board_index][row][col];
                    else    target[row][col]=-1;
                    std::cout << std::setw(3) << target[row][col];
            }
            std::cout << '\n';
        }

     
}



/***************** data processing completed**********************/

bool targeting(){
    for(int row=0; row<8; row++)
        for(int col=0; col<8; col++)
            if(board[row][col]!=-1){
                if(board[row][col]!=target[row][col])    return false;
            }
   // printf("1121221\n");
    return true;
}


bool sub_targeting(int sminrow, int smaxrol, int smincol, int smaxcol){
    for(int row=sminrow; row < smaxrol; row++)
        for(int col = smincol; col < smaxcol; col++)
            if(board[row][col]!=-1){
                if(board[row][col]!=target[row][col])    return false;
            }
    return true;
}

void conver(char ans[8][8], int board[8][8]){
    for(int i=0; i<8 ;i++)
        for(int j=0;j<8;j++){
            if(board[i][j]!=-1)
                ans[i][j] = static_cast<char>(board[i][j]);
            else
                ans[i][j]=' ';
        }
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            std::cout << std::setw(3) << ans[row][col];
        }
        std::cout << '\n';
    }
    
}


bool comp(int board[8][8], int target[8][8]){
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            if(board[row][col] != target[row][col])    return false;
        }
    }
    return true;
}



void isBoard()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            std::cout << std::setw(3) << board[row][col];
        }
        std::cout << '\n';
    }
}

void issolution()
{
    std::cout << pieceOrient[0] << pieceRow[0]+1 << pieceCol[0]+1;
    for (int n = 1; n < num_pie; ++n)
    {
        std::cout << '-' << pieceOrient[n] << pieceRow[n]+1 << pieceCol[n]+1;
    }
    std::cout << ' ' << solutionCount << std::endl;
}
//pieceMax[lastPiece + 1];
bool testPiece(int row, int col, int orient)
{
    for (int r = 0; r != 5; ++r)
    {
        for (int c = 0; c != 5; ++c)
        {
            const int val = piece[pieceIdx[lastPiece + 1] + orient][r][c];
            if (val != -1)
            {
                if (7 < row + r || 7 < col + c || board[row + r][col + c] != -1)
                {
                    return false;
                }
            }
        }
    }

    return true;
}


void placePiece(int row, int col, int orient)
{
    ++lastPiece;
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < 5; c++)
        {
            const int val = piece[pieceIdx[lastPiece] + orient][r][c];
            if (val != -1)
            {
                board[row + r][col + c] = val;
              //  std::cout << val << "\n" <<std::endl;
            }
        }
    }
  //  std::cout  << "\n" << row << "\n" << col << "\n" << orient << "\n"<<std::endl;
    pieceRow[lastPiece] = row;
    pieceCol[lastPiece] = col;
    pieceOrient[lastPiece] = orient;
    used[lastPiece]=true;
}

void removePiece()
{
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < 5; c++)
        {
            const int val = piece[pieceIdx[lastPiece] + pieceOrient[lastPiece]][r][c];
            if (val != -1)
            {
                board[pieceRow[lastPiece] + r][pieceCol[lastPiece] + c] = -1;
            }
        }
    }
    pieceRow[lastPiece] = -1;
    pieceCol[lastPiece] = -1;
    pieceOrient[lastPiece] = -1;
    used[lastPiece] = false;
    --lastPiece;
}

int mark(int row, int col, int markVal)
{
    int markCount = 0;
    
    
    if (board[row][col] == -1)
    {
        board[row][col] = markVal;
        ++markCount;
        if(row<minrow_split[n_split])
            minrow_split[n_split]=row;
        if(row>maxrow_split[n_split])
            maxrow_split[n_split]=row;
        if(col<mincol_split[n_split])
            mincol_split[n_split]=col;
        if(col>maxcol_split[n_split])
            maxcol_split[n_split]=col;

        if (0 < row && board[row-1][col] == -1)
        {
            markCount += mark(row - 1, col, markVal);
        }
        if (row < 7&& board[row+1][col] == -1)
        {
            markCount += mark(row + 1, col, markVal);
        }
        if (0 < col&& board[row][col-1] == -1)
        {
            markCount += mark(row, col - 1, markVal);
        }
        if (col < 7&& board[row][col+1] == -1)
        {
            markCount += mark(row, col + 1, markVal);
        }
        
    }

    return markCount;
}

bool branchkiller()
{
    bool bResult = false;
    int markVal = -2;
    int totalMarkCount = 0;
    int minarea=64;
    for(int i=0; i<num_pie; i++)
        if(used[i] != true)
            if(area[i]<minarea)
                minarea=area[i];

    for (int row = 0; row <= 7; ++row)
    {
        for (int col = 0; col <= 7; ++col)
        {
            const int markCount = mark(row, col, markVal);
            if (markCount!=0)
            {
//isBoard();
//       std::cout<< markVal<< " "<<markCount <<"  "<<n_split <<"\n" << std::endl;
                
                --markVal;
                area_split[n_split]=markCount;
                n_split++;
            }
            else
            {
                area_split[n_split]=0;
                minrow_split[n_split]=7;        
                maxrow_split[n_split]=0;
                mincol_split[n_split]=7;        
                maxcol_split[n_split]=0;
                continue;
            }
            if (markCount < minarea)
            {
               //	 printf("%d  ", markCount);
/*isBoard();
std::cout <<"\n" << std::endl;
std::cout << markCount<< "   "<< minarea << "  " << "\n"<< std::endl;*/
                bResult = true;
                goto cleanup;
            }
            
        }
    }
    
        

cleanup:
    

    for (int row = 0; row <= 7; ++row)
    {
        for (int col = 0; col <= 7; ++col)
        {
            if (board[row][col] < 0)
            {
                board[row][col] = -1;
            }
        }
    }
    int temp=64;
    minarea_split = 0;
    for(int i=0;i<n_split;i++){
        if(area_split[i]<temp){
             temp = area_split[i];
             minarea_split = i;
        } 
    }

    return bResult;
}

void Piecereplace(int row, int col, int orient)
{
    
    
    if (!testPiece(row, col, orient))
    {
        return;
    }
    
    placePiece(row, col, orient);
    //isBoard();
   // system("read");
   // std::cout<<"    "<<lastPiece<<"\n" << std::endl;
  /* if(lastPiece>=num_pie-3){
        isBoard();
        std::cout<<"\n" << std::endl;
    }*/
    if(lastPiece >= num_pie-2)
    {
        
        issolution();
        isBoard();
      //  for(int i=0; i< num_pie-2; i++)  std::cout<< pieceMax[i]<<"\n" << std::endl;
        std::cout<< lastPiece <<"\n" << std::endl;

    

        if(comp(board, target)==true){
                std::cout <<  "A solution is here!!" << ans[0][0] <<  std::endl;
                solutionCount++;
                conver(ans, board);
                if(first==0){
                    first=1;
                    std::time_t now = std::time(NULL);
                    std::tm * ptm = std::localtime(&now);
                    std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm); 
                }
              //  system("read");
              //  exit (1);
            }
    }
    else
    {   n_split=0;
        minrow_split[0]=7;        
        maxrow_split[0]=0;
        mincol_split[0]=7;        
        maxcol_split[0]=0;

        if ( !branchkiller() & targeting() )
        {
   /* isBoard();
       std::cout <<"\n" << std::endl;
    std::cout << n_split << "   "<<minarea_split << "  " << area_split[minarea_split] << "."<< std::endl;
    std::cout << minrow_split[minarea_split] << "  " << maxrow_split[minarea_split] << "." << std::endl;
    std::cout << mincol_split[minarea_split] << "  " << maxcol_split[minarea_split] << "." << std::endl;*/
            int rowMax = maxrow_split[minarea_split] - pieceMin[lastPiece + 1] + 1;
            int rowMin = minrow_split[minarea_split];
            int colMax = maxcol_split[minarea_split] - pieceMin[lastPiece + 1] + 1;
            int colMin = mincol_split[minarea_split];
            bool fit = false;
//orientMax[lastPiece + 1]
            for (int orient = 0; orient < orientMax[lastPiece + 1]; orient++)
            {
                for (int row = rowMin; row <= rowMax; row++)
                {
                    for (int col = colMin; col <= colMax; col++)
                    {
                        Piecereplace(row, col, orient);
                   //     sub_targeting ( minrow,  maxrol,  mincol,  maxcol)
                    }
                }
            }
        }
    }

    removePiece();
}

void Sweep(int row, int col)
{
 /*  if (!testPiece(row, col, 0))
    {
        return;
    }*/

/*placePiece(2, 2, 0);
placePiece(0, 3, 0);
placePiece(0, 5, 0);
placePiece(2, 0, 0);
placePiece(0, 0, 3);
placePiece(6, 0, 0);
placePiece(4, 3, 0);
placePiece(4, 6, 0);
 isBoard();
   system("read");*/

    
  /*  n_split=0;
    minrow_split[0]=7;        
    maxrow_split[0]=0;
    mincol_split[0]=7;        
    maxcol_split[0]=0;*/
    if (  targeting())
    {

        /*    const int rowMax = maxrow_split[minarea_split] - pieceMin[lastPiece + 1];
            const int rowMin = minrow_split[minarea_split];
            const int colMax = maxcol_split[minarea_split] - pieceMin[lastPiece + 1];
            const int colMin = mincol_split[minarea_split];*/
        for (int orient = 0; orient < orientMax[lastPiece + 1]; orient++)
        {
          
                for (int row = 0; row <= 8-1; row++)
                {
                    for (int col = 0; col <= 8-1; col++)
                    {
                        Piecereplace(row, col, orient);
                    
                    } 
                }

        }
    }


}


int main(int argc, char* argv[])
{
    std::cout << "Algorithm Term Project\n"<< std::endl;
    int first=0;
    std::time_t time1;
    std::time(&time1);

    std::time_t now = std::time(NULL);
    std::tm * ptm = std::localtime(&now);
    char buffer2[32];
    std::strftime(buffer2, 32, "%a, %d.%m.%Y %H:%M:%S", ptm); 

    data_process();

   // for(int i=0; i< num_pie; i++)  std::cout<<pieceMax[i]<<"\n" << std::endl;


    Sweep(0, 0);

    std::time_t time2;
    std::time(&time2);
    double seconds = std::difftime(time2, time1);
    std::cout <<  "All solutions found in " << seconds << " seconds." <<"\n"<< std::endl;
    std::cout <<  " Start: " << buffer2<<"\n" << " A solution found in." << buffer <<std::endl;

    return 0;
}