# How to Run the Shell Script

If changes are made to your html or css you will need to regenerate it by running the `hex2c.sh` shell script.

### OSX/Linux

Just run the command on the file you want to regenerate, piping the output to the destination file. For example, to regenerate the range finder scanner's `index.html` file, you would run the following command:

```
./docs/cpp/bin/hex2c.sh ./range-finder-scanner/cpp/html/index.html > ./range-finder-scanner/cpp/src/html.h
```

The resulting `html.h` contains the hex encoded version of the `index.html` as a C header file. This allows a single file executable with all assets included in the file itself, hence no external dependencies.

### Windows

1. Go to https://cygwin.com/
![](./../../images/cpp/shell-script.png)

2. Go to the downloads section.
![](./../../images/cpp/shell-script2.png)

3. Download and run the exe.
![](./../../images/cpp/shell-script3.png)

4. Only use default install!!! Download will be several gigabytes if you install all.
![](./../../images/cpp/shell-script4.png)

5. Go to windows control panel and click on system.
![](./../../images/cpp/shell-script5.png)

6.click on advance settings.
![](./../../images/cpp/shell-script6.png)

7.Under advanced, click on enviromental variables.
![](./../../images/cpp/shell-script7.png)

8. Find path on the second box, click it, then click edit.
![](./../../images/cpp/shell-script8.png)

9. Add ";C\cygwin64\bin" to you path. (assuming this is where you intalled cygwin)
![](./../../images/cpp/shell-script9.png)

10. Open your windows command promt.
![](./../../images/cpp/shell-script10.png)

11. Find the directory the your shell script command is as well as your html and css files. For ease of use I put them in the same folder on my desktop. Now type "hex2c.sh index.html"
![](./../../images/cpp/shell-script11.png)

12. You may have to change the height and wisth of your command prompt for everything to fit.
![](./../../images/cpp/shell-script12.png)

13. Copy the hex code completly. Right click at the top bar of the command promt. Click edit -> mark.
![](./../../images/cpp/shell-script13.png)

14. You can now hightlight the whole file. Press enter to copy.
![](./../../images/cpp/shell-script14.png)

15. Finally open a new text document and paste the code inside. Then rename the document html.h and you now have the file you need if you modified your html or css.
![](./../../images/cpp/shell-script15.png)
