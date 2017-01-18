/*
MIT License

Copyright (c) 2017  Joe Hood

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include<vector>
#include<string>

#define MAX_CURVES 10

using namespace std;

class AsciiPlotter
{
private:
	static const char _default_markers[10];
	int _width;
	int _height;
	int _curves;
	string _title;
	string _xlabel;
	string _ylabel;
	bool _legend;
	vector<double> _xdata;
	vector<double> _ydata[MAX_CURVES];
	string _labels[MAX_CURVES];
	char _markers[MAX_CURVES];
public:
	AsciiPlotter();
	AsciiPlotter(string title);
	AsciiPlotter(string title, int width, int height);
	~AsciiPlotter();
	void plot(const char *plotfile, const char *datafile);
	void addPlot(vector<double> xdata, vector<double> ydata, string label, char marker);
	void show();
	void xlabel(string label);
	void ylabel(string label);
	void legend();
	void example();
};

