/**
 * @file schedule.cpp
 * Exam scheduling using graph coloring
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "schedule.h"
#include "utils.h"
#include <algorithm>

/**
 * Given a filename to a CSV-formatted text file, create a 2D vector of strings where each row
 * in the text file is a row in the V2D and each comma-separated value is stripped of whitespace
 * and stored as its own string. 
 * 
 * Your V2D should match the exact structure of the input file -- so the first row, first column
 * in the original file should be the first row, first column of the V2D.
 *  
 * @param filename The filename of a CSV-formatted text file. 
 */
V2D file_to_V2D(const std::string & filename){
    // Your code here!
    std::string fileString = file_to_string(filename);
    std::vector<std::string> lines;

    V2D ret;
    split_string(fileString, '\n', lines);
    for(unsigned int i = 0; i < lines.size(); i++){
        std::vector<std::string> temp;
        split_string(lines.at(i), ',', temp);
        
        for(unsigned int j = 0; j < temp.size(); j++){
            temp.at(j) = trim(temp.at(j));
        }
        
        ret.push_back(temp);
    }
    return ret;
}

/**
 * Given a course roster and a list of students and their courses, 
 * perform data correction and return a course roster of valid students (and only non-empty courses).
 * 
 * A 'valid student' is a student who is both in the course roster and the student's own listing contains the course
 * A course which has no students (or all students have been removed for not being valid) should be removed
 * 
 * @param cv A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param student A 2D vector of strings where each row is a student ID followed by the courses they are taking
 */
V2D clean(const V2D & cv, const V2D & student){
    // YOUR CODE HERE
    V2D ret;
    for(unsigned int i = 0; i < cv.size(); i++){
        std::vector<std::string> temp;
        std::vector<std::string> curClass = cv.at(i);
        std::string className = curClass.at(0);
        
        for(unsigned int j = 1; j < curClass.size(); j++){
            for(unsigned int k = 0; k < student.size(); k++){
                if((student.at(k)).at(0) == curClass.at(j)){
                    for(unsigned int x = 0; x < student.at(k).size(); x++){
                        if(student.at(k).at(x) == className){
                            temp.push_back(curClass.at(j));
                        }
                    }
                }
            }
        }
        if(!temp.empty()){
            temp.insert(temp.begin(), className);
            ret.push_back(temp);
        }
        
    }
    return ret;
}

/**
 * Given a collection of courses and a list of available times, create a valid scheduling (if possible).
 * 
 * A 'valid schedule' should assign each course to a timeslot in such a way that there are no conflicts for exams
 * In other words, two courses who share a student should not share an exam time.
 * Your solution should try to minimize the total number of timeslots but should not exceed the timeslots given.
 * 
 * The output V2D should have one row for each timeslot, even if that timeslot is not used.
 * 
 * As the problem is NP-complete, your first scheduling might not result in a valid match. Your solution should 
 * continue to attempt different schedulings until 1) a valid scheduling is found or 2) you have exhausted all possible
 * starting positions. If no match is possible, return a V2D with one row with the string '-1' as the only value. 
 * 
 * @param courses A 2D vector of strings where each row is a course ID followed by the students in the course
 * @param timeslots A vector of strings giving the total number of unique timeslots
 */
V2D schedule(const V2D &courses, const std::vector<std::string> &timeslots){
    // Your code here!
    V2D ret;

    std::vector<std::string> classes;
    for (unsigned int i = 0; i < courses.size(); i++){
        classes.push_back((courses.at(i)).at(0));
    }
    std::vector<std::vector<int>> adj;
    adj.resize(courses.size());
    for (unsigned int i = 0; i < courses.size(); i++){
        std::vector<std::string> curClass = courses.at(i);
        for(unsigned int j = 1; j < curClass.size(); j++){
            for(unsigned int k = 0; k < courses.size(); k++){
                if(k == i){
                    continue;
                }
                std::vector<std::string> nextClass = courses.at(k);
                for(unsigned int l = 1; l < nextClass.size(); l++){
                    
                    if(curClass.at(j) == nextClass.at(l)){
                        adj[i].push_back(k);
                    }
                    //std::cout << nextClass.size() - l;
                }
            }
        }
    }
    std::vector<int> queue;
    std::map<int, int> colors;
    std::vector<bool> visited;
    std::vector<int> touching;
    visited.resize(courses.size());
    int curr;
    int curColors;
    for(unsigned int i = 0; i < adj.size(); i++){
        colors.clear();
        for (unsigned int v = 0; v < visited.size(); v++){
            visited[v] = false;
        }
        curColors = 0;
        queue.clear();
        queue.push_back(i);
        while(!queue.empty()){
            curr = queue.front();
            //std::cout << curr << ";";
            visited[curr] = true;
            queue.erase(queue.begin());
            touching.resize(0);
            for(int vertex : adj[curr]){
                if(colors.find(vertex) != colors.end()){
                    bool already = false;
                    for (unsigned int t = 0; t < touching.size(); t++){
                        if(touching[t] == colors[vertex]){
                            already = true;
                        }
                    }
                    if (!already){
                        touching.push_back(colors[vertex]);
                    }
                    
                    //std::cout<< " (" << vertex << "," <<colors[vertex]<<")";
                }
                if(visited[vertex] == false){
                    queue.push_back(vertex);
                    visited[vertex] = true;
                }
            }
            //std::cout<<"\n";
            if(int(touching.size()) >= curColors){
                curColors++;
                colors[curr] = curColors;
            } else if (touching.empty()){
                colors[curr] = 55;
            }
            else{
                for(int c = 1; c <= curColors; c++){
                    bool test = true;
                    for(unsigned int j = 0; j < touching.size(); j++){
                        if(c == touching[j]){
                            test = false;
                        }
                    }
                    if(test){
                        colors[curr] = c;
                    }
                }
            }
        }
        if(curColors <= int(timeslots.size())){
            break;
        }
    }
    /*std::cout << "\n";
    for(unsigned int i = 0; i < courses.size(); i++){
        std::cout << i << " : " << colors[i] << std::endl;
    }
    */
    if(curColors > int(timeslots.size())){
        ret.resize(1);
        ret[0].resize(1);
        ret[0][0] = "-1";
        return ret;
    }
    ret.resize(timeslots.size());
    for(int i = 0; i < curColors; i++){
        ret[i].push_back(timeslots[i]);
        for(unsigned int vertex = 0; vertex < classes.size(); vertex++){
            if(colors[vertex] == i + 1){
                ret[i].push_back(classes[vertex]);
            }
        }
    }
    return ret;
}