#include <cmath>
#include <iostream>
using namespace std;
constexpr bool verbose = true;
int tessCount = 0;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

bool * findConcavity(int indSize, double * x, double * y)//Determines concavity of each point
{
    if(verbose)
        cout << "Finding Concavity" << endl;
    int nextI, next2I;
    double dx1, dx2, dy1, dy2;
    double crossProducts[indSize];
    bool isConcave[indSize];
    
    for(int i = 0; i < indSize; i++)//Find crossproducts of points to determine concavity 
    {
        nextI = (i+1)%(indSize);
        next2I = (i+2)%(indSize);
        dx1 = x[nextI]-x[i];
        dy1 = y[nextI]-y[i];
        dx2 = x[next2I]-x[nextI];
        dy2 = y[next2I]-y[nextI];
        crossProducts[nextI] = dx1 * dy2 - dy1 * dx2;
        if(verbose)
            cout << "Cross Product: " << crossProducts[nextI] << endl;

        if(crossProducts[nextI] != 0)
        {
            if(crossProducts[nextI] > 0)
            {
                isConcave[i] = true;
            }
            else
            {
                isConcave[i] = false;
            }
        }
    }
    if(verbose)
    {
        cout << "\n\n***Points        : ";
        for (int i = 0; i < indSize; i++) {
            if(sgn(crossProducts[i]) > 0)
                cout << i << ' ';
            else
                cout << ' ' << i << ' ';
        }
        cout << "\n***Cross products: ";
        for (int i = 0; i < indSize; i++) {
            cout << sgn(crossProducts[i]) << ' ';
        }
        cout << '\n';
    }

    bool isConvex = true;
    //Special check in case all vertices are marked as concave, which is impossible
    //All vertices are corrected to convex
    for(int i = 1; i < indSize; i++)
    {
        if(sgn(crossProducts[i]) != sgn(crossProducts[i-1]))
        {
            isConvex = false;
            break;
        }
    }
    if(isConvex)
    {
        for(int i = 0; i < indSize; i++)
        {
            isConcave[i] = false;
        }
        cout << "\nAll Convex\n" << endl;
    }
    bool * a = isConcave;
    return a;
}

//Tesselator Function 
//Returns a vector of triangle coordinates, creates duplicates and is inefficient
vector<float> tessellate(int indSize, double * x, double * y, bool * isConcave)
{
    tessCount++;
    if(verbose)
        cout << "Tesselation Count: " << tessCount << endl;
    int concavePoints[indSize];
    int conSize = 0;
    int tessPoint = 0;
    vector<float> triangles;
    triangles.reserve(indSize*4);
    if(verbose)
        cout << "Tesselating" << endl;
    for(int i = 0; i < indSize; i++)//
    {
        if(isConcave[i] == true)
        {
            concavePoints[conSize] = i;
            conSize++;
        }
    }
    double concave_x[conSize];//Concave Points
    double concave_y[conSize];
    for(int i = 0; i < conSize; i++)//Create new set of points consisting of only concave points
    {
        concave_x[i] = x[concavePoints[i]];
        concave_y[i] = y[concavePoints[i]];
    }
    if(conSize > 1)//Tesselate pockets between concave points
    {
        if(verbose)
            cout << "Large Segment" << endl;
        int nextI, startPoint, endPoint, new_size;
        for(int i = 0; i < conSize; i++)
        {
            if(verbose)
                cout << "Entering Main Loop of Large Segment" << endl;
            nextI = (i+1)%conSize;
            startPoint = concavePoints[i];
            endPoint = concavePoints[nextI];
            if(endPoint > startPoint)
                new_size = endPoint - startPoint + 1;
            else
                new_size = (indSize - startPoint) + endPoint + 1;
            if(verbose)
                cout << "New Size: " << new_size << endl;
            double new_x[new_size];
            double new_y[new_size];
            bool * isConcave2;
            if(verbose)
                cout << "\n\nFirst Nested Loop of Large Segment" << endl;
            for(int j = 0; j < new_size; j++)
            {
                
                new_x[j] = x[(j+startPoint)%indSize];
                new_y[j] = y[(j+startPoint)%indSize];
                if(verbose)
                {  
                    cout << "(" << new_x[j] << ", " << new_x[j] << ")" << endl;
                }
            }
            if(verbose)
                cout << "Checking Concavity of Large Segment" << endl;
            isConcave2 = findConcavity(new_size, new_x, new_y);

            //Adding Triangles to List
            if(verbose)
                cout << "Adding Triangles to List" << endl;
            vector<float> new_triangles = tessellate(new_size, new_x, new_y, isConcave2);//Recursion
            triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
        }
        cout << "\n\nConcavity: " << findConcavity(conSize, concave_x, concave_y) << endl;
        vector<float> new_triangles = tessellate(conSize, concave_x, concave_y, findConcavity(conSize, concave_x, concave_y));//Recursion
        triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
    }
    else
    {
        if(verbose)
            cout << "Small Segment" << endl;

        if(conSize == 1)
            tessPoint = concavePoints[0];
        else
            tessPoint = 0;
        double triangle[6];
        int nextI;
        for(int i = 0; i < indSize; i++)
        {
            nextI = (i+1)%indSize;
            if(i != tessPoint && nextI != tessPoint)
            {
                triangles.push_back(x[i]);
                triangles.push_back(y[i]);
                triangles.push_back(x[nextI]);
                triangles.push_back(y[nextI]);
                triangles.push_back(x[tessPoint]);
                triangles.push_back(y[tessPoint]);
            }
            
        }
    }
    if(verbose)
        cout << "Finished Tesselating Segment" << "\tSize: " << triangles.size()<< endl;
    return triangles;
}

//Fan Tessellator
//Returns a Vector of a Vector of triangle fan coordinates
//Meant to reduce the number of duplicate coordinates
/*vector<vector<float>> fanTessellate(int indSize, double * x, double * y, bool * isConcave)
{
    tessCount++;
    if(verbose)
        cout << "Tesselation Count: " << tessCount << endl;
    int concavePoints[indSize];
    int conSize = 0;
    int tessPoint = 0;
    vector<vector<float>> triangleFans;
    triangles.reserve(indSize*4);
    if(verbose)
        cout << "Tesselating" << endl;
    for(int i = 0; i < indSize; i++)//
    {
        if(isConcave[i] == true)
        {
            concavePoints[conSize] = i;
            conSize++;
        }
    }
    double concave_x[conSize];//Concave Points
    double concave_y[conSize];
    for(int i = 0; i < conSize; i++)//Create new set of points consisting of only concave points
    {
        concave_x[i] = x[concavePoints[i]];
        concave_y[i] = y[concavePoints[i]];
    }
    if(conSize > 1)//Tesselate pockets between concave points
    {
        if(verbose)
            cout << "Large Segment" << endl;
        int nextI, startPoint, endPoint, new_size;
        for(int i = 0; i < conSize; i++)
        {
            if(verbose)
                cout << "Entering Main Loop of Large Segment" << endl;
            nextI = (i+1)%conSize;
            startPoint = concavePoints[i];
            endPoint = concavePoints[nextI];
            if(endPoint > startPoint)
                new_size = endPoint - startPoint + 1;
            else
                new_size = (indSize - startPoint) + endPoint + 1;
            if(verbose)
                cout << "New Size: " << new_size << endl;
            double new_x[new_size];
            double new_y[new_size];
            bool * isConcave2;
            if(verbose)
                cout << "\n\nFirst Nested Loop of Large Segment" << endl;
            for(int j = 0; j < new_size; j++)
            {
                
                new_x[j] = x[(j+startPoint)%indSize];
                new_y[j] = y[(j+startPoint)%indSize];
                if(verbose)
                {  
                    cout << "(" << new_x[j] << ", " << new_x[j] << ")" << endl;
                }
            }
            if(verbose)
                cout << "Checking Concavity of Large Segment" << endl;
            isConcave2 = findConcavity(new_size, new_x, new_y);

            //Adding Triangles to List
            if(verbose)
                cout << "Adding Triangles to List" << endl;
            vector<float> new_triangles = tessellate(new_size, new_x, new_y, isConcave2);//Recursion
            triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
        }
        cout << "\n\nConcavity: " << findConcavity(conSize, concave_x, concave_y) << endl;
        vector<float> new_triangles = tessellate(conSize, concave_x, concave_y, findConcavity(conSize, concave_x, concave_y));//Recursion
        triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
    }
    else
    {
        if(verbose)
            cout << "Small Segment" << endl;

        if(conSize == 1)
            tessPoint = concavePoints[0];
        else
            tessPoint = 0;
        double triangle[6];
        int nextI;
        for(int i = 0; i < indSize; i++)
        {
            nextI = (i+1)%indSize;
            if(i != tessPoint && nextI != tessPoint)
            {
                triangles.push_back(x[i]);
                triangles.push_back(y[i]);
                triangles.push_back(x[nextI]);
                triangles.push_back(y[nextI]);
                triangles.push_back(x[tessPoint]);
                triangles.push_back(y[tessPoint]);
            }
            
        }
    }
    if(verbose)
        cout << "Finished Tesselating Segment" << "\tSize: " << triangles.size()<< endl;
    return triangles;
}*/