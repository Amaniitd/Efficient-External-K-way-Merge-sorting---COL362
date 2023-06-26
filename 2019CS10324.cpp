#include <bits/stdc++.h>

using namespace std;


int external_merge_sort_withstop(const char* input, const char* output, const long key_count, const int k = 2, const int num_merges = 0) {

    ifstream input_file(input);
    vector<string> buffer;
    buffer.reserve(102400);
    int run_count = 0;
    int linecount = 0;
    string line;
    while (true) {
        if (input_file.eof()) {
            break;
        }
        if (buffer.size() >= 102400) {
            break;
        }

        if (getline(input_file, line) && linecount < key_count)
        {
            buffer.push_back(line);
            linecount+=1;
        }
        
        if (buffer.size() == 102400 || input_file.eof()) {
            sort(buffer.begin(), buffer.end());
            stringstream filename;
            filename << "temp.0." << ++run_count;
            ofstream output_file(filename.str());
            for (int i = 0; i < buffer.size(); i++) {
                output_file << buffer[i] << endl;
            }
            buffer.clear();
            output_file.close();
        }
    }
    input_file.close();
    int merge_count = 0;
    int merge_step = 0;
    string finalfile;
    while (true)
    {
        if (run_count <= 1) {
            break;
        }
        vector<string> output_buffer;
        output_buffer.reserve(102400);
        
        int curr_count = 1;
        int file_count = 1;
        int num_files = 0;
        while (run_count >= curr_count) {
            int merge_size;
            if (k < run_count - curr_count + 1)
            {
                merge_size = k;
            }
            else
            {
                merge_size = run_count - curr_count + 1;
            }
            vector<ifstream> input_files(merge_size);
            vector<string> runs(merge_size);
            for (int i = 0; i < merge_size; i++) {
                stringstream filename;
                filename << "temp." << merge_count << "." << (curr_count + i);
                input_files[i].open(filename.str());
                getline(input_files[i], runs[i]);
            }
            stringstream out_filename;
            out_filename << "temp." << (merge_count + 1) << "." << file_count++;
            finalfile = out_filename.str();
            ofstream output_file(out_filename.str());
            while (true) {
                int min_idx = -1;
                string min_str;
                for (int i = 0; i < merge_size; i++) {
                    if (input_files[i].eof()) {
                        continue;
                    }
                    if (min_idx == -1 || runs[i] < min_str) {
                    
                        min_idx = i;
                        min_str = runs[i];
                    }
                }
                if (min_idx == -1) {
                    break;
                }

                output_buffer.push_back(min_str);

                if (output_buffer.size() == 102400) {
                    for (int i = 0; i < output_buffer.size(); i++) {
                        output_file << output_buffer[i] << endl;
                    }
                    output_buffer.clear();
                }
                getline(input_files[min_idx], runs[min_idx]);
            }
            for (int i = 0; i < output_buffer.size(); i++) {
                output_file << output_buffer[i] << endl;
            }
            output_buffer.clear();
            output_file.close();
            num_files+=1;
            curr_count += merge_size;
            merge_step+=1;
            if(num_merges) {
                if(merge_step == num_merges) {
                    return merge_step;
                }
            }
        }
        run_count = num_files;
        merge_count+=1;
    }    
    
    char* final_name = new char[finalfile.size() + 1];
    strcpy(final_name, finalfile.c_str());
    rename(final_name, output);
    return merge_step;
}
