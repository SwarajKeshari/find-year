# find-year


## How to run

1. Clone this repository on your system.
2. Open `solution.cpp` and update this line no. 13:
   ```cpp
   
   fs::path base = "absolute path to data folder";
   
   ```
   Replace "absolute path to data folder" with the absolute path to the data folder on your system.

   Example (Linux):
   ```
   
   fs::path base = "/home/username/projects/find-year/data";
   
   ```
   Example (Windows):
   ```
   
   fs::path base = "C:/Users/username/Desktop/find-year/data";
   
   ```

3. Compile and run:
   ```
   g++ solution.cpp -o find-year && ./find-year
   ```

