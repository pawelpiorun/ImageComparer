#include <string>
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

int main()
{
    std::cout << "Hello in image comparer! This .exe will find and write info about duplicates in specified folder.\n";
    std::cout << "Please specify folder: \n";
    std::string folder;
    std::getline(std::cin, folder);

    std::vector<std::string> paths;
    auto dirIterator = std::filesystem::directory_iterator(folder);
    for (const auto& path : dirIterator)
        paths.push_back(path.path().string());

    std::system("cls");
    int totalCount = paths.size();

    long options = 1;
    for (int i = totalCount - 1; i > 0; i--)
        options += i;

    double tolerance = 0.001;
    int duplicates = 0;
    int skipped = 0;
    int counter = 0;

    std::stringstream str;
    for (int i = 0; i < paths.size(); i++)
    {
        cv::Mat img = cv::imread(paths[i]);

        // start with next image to avoid double checks
        for (int j = i + 1; j < paths.size(); j++)
        {
            std::system("cls");
            std::cout << "Progress: " << counter << "/" << options << std::endl;
            counter++;
            if (i == j) continue;
            
            cv::Mat other = cv::imread(paths[j]);
            try
            {
                cv::Mat sub;
                cv::absdiff(img, other, sub);

                auto mean = cv::mean(sub);
                double min = 0, max = 0;
                cv::minMaxLoc(sub, &min, &max);

                if (std::abs(mean.val[0]) <= tolerance
                    && std::abs(mean.val[1]) <= tolerance
                    && std::abs(mean.val[2]) <= tolerance
                    && std::abs(mean.val[3]) <= tolerance
                    && std::abs(min) <= tolerance
                    && std::abs(max) <= tolerance)
                {
                    duplicates++;
                    str << "[" << duplicates << "] Found duplicate:\n" << paths[i] << std::endl << paths[j] << std::endl;
                }
            }
            catch (std::exception x)
            {
                str << "Cannot compare:\n" << paths[i] << std::endl << paths[j] << std::endl;
                skipped++;
            }
        }
    }

    std::system("cls");
    std::cout << str.str() << std::endl;
    std::cout << std::endl << "Total duplicates found: " << duplicates << std::endl;
    std::getchar();
}