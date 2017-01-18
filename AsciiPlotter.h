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

