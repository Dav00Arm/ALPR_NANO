#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <unordered_map>
#include <opencv2/opencv.hpp>

/*
    This file's content is for checking whether the license plate is
    in drawn zone. If yes contiune and crop lines.
*/

// Point type for drawing custom qarankyunner.
struct point {
    int x, y;
};

// line type for drawing custom qarankyunner.
struct line {
    point p1, p2;
};

// Check whether point is on the line or not
bool onLine(line l1, point p)
{
    if (p.x <= std::max(l1.p1.x, l1.p2.x)
        && p.x <= std::min(l1.p1.x, l1.p2.x)
        && (p.y <= std::max(l1.p1.y, l1.p2.y)
            && p.y <= std::min(l1.p1.y, l1.p2.y)))
        return true;
 
    return false;
}

// Get the direction of lines (chem hishum xia petq) 
int direction(point a, point b, point c)
{
    int val = (b.y - a.y) * (c.x - b.x)
              - (b.x - a.x) * (c.y - b.y);
 
    if (val == 0)
 
        // Colinear
        return 0;
 
    else if (val < 0)
 
        // Anti-clockwise direction
        return 2;
 
    // Clockwise direction
    return 1;
}

// Check the intersection of qarankyuns.
bool isIntersect(line l1, line l2)
{
    // Four direction for two lines and points of other line
    int dir1 = direction(l1.p1, l1.p2, l2.p1);
    int dir2 = direction(l1.p1, l1.p2, l2.p2);
    int dir3 = direction(l2.p1, l2.p2, l1.p1);
    int dir4 = direction(l2.p1, l2.p2, l1.p2);
 
    // When intersecting
    if (dir1 != dir2 && dir3 != dir4)
        return true;
    // When p2 of line2 are on the line1
    if (dir1 == 0 && onLine(l1, l2.p1))
        return true;
    // When p1 of line2 are on the line1
    if (dir2 == 0 && onLine(l1, l2.p2))
        return true;
    // When p2 of line1 are on the line2
    if (dir3 == 0 && onLine(l2, l1.p1))
        return true;
    // When p1 of line1 are on the line2
    if (dir4 == 0 && onLine(l2, l1.p2))
        return true;
 
    return false;
}

// Check if point is in polygon
bool checkInsidePoint(std::vector<point> poly, point p)
{
    // When polygon has less than 3 edge, it is not polygon
    
    // Create a point at infinity, y is same as point p
    line exline = { p, { 9999, p.y } };
    int count = 0;
    int i = 0;
    do {
        // Forming a line from two consecutive points of
        // poly
        line side = { poly[i], poly[(i + 1) % 4] };
        if (isIntersect(side, exline)) {
 
            // If side is intersects exline
            if (direction(side.p1, p, side.p2) == 0)
                return onLine(side, p);
            count++;
        }
        i = (i + 1) % 4;
    } while (i != 0);
 
    // When count is odd
    return count & 1;
}
// Convert std::vector<cv::Point> to std::vector<point>. 
std::vector<point> convert_polygon(std::vector<cv::Point> box){

    std::vector<point> polygon = {{box[0].x, box[0].y}, {box[1].x, box[1].y},
                                  {box[2].x, box[2].y}, {box[3].x, box[3].y}};

    return polygon;
}

// Convert std::vector<std::vector<int>> to std::vector<point>. 
std::vector<point> convert_polygon(std::vector<std::vector<int>> box){
    std::vector<point> polygon = {{box[0][0], box[0][1]}, {box[1][0], box[0][1]},
                                  {box[1][0], box[1][1]}, {box[0][0], box[1][1]}};

    return polygon;
}

// Check if license plate is in drawn qarankyun box (std::vector<cv::Point>). 
bool checkInside(std::vector<cv::Point> spot, std::vector<std::vector<int>> plate_coords){
    std::vector<point> spot_point = convert_polygon(spot);
    std::vector<point> plate_point = convert_polygon(plate_coords);
    
    for(int i=0; i<4; i++){
        if(!checkInsidePoint(spot_point, plate_point[i])){
            return false;
        }
    }
    return true;
}

// Check if license plate is in drawn qarankyun box (std::vector<std::vector<int>>). 
bool checkInside(std::vector<std::vector<int>> spot, std::vector<std::vector<int>> plate_coords){
    std::vector<point> spot_point = convert_polygon(spot);
    std::vector<point> plate_point = convert_polygon(plate_coords);
  
    for(int i=0; i<4; i++){
        if(!checkInsidePoint(spot_point, plate_point[i])){
            return false;
        }
    }
    return true;
}
