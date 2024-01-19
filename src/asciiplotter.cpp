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

#include "asciiplotter.h"

using namespace std;

int max(vector<int> data) {
  int xmax = data[0];
  for (int i = 1; i < sizeof(data); i++) {
    if (data[i] > xmax) {
      xmax = data[i];
    }
  }
  return xmax;
}

int min(vector<int> data) {
  int xmin = data[0];
  for (int i = 1; i < data.size(); i++) {
    if (data[i] < xmin) {
      xmin = data[i];
    }
  }
  return xmin;
}

double max(vector<double> data) {
  double xmax = data[0];
  for (int i = 1; i < data.size(); i++) {
    if (data[i] > xmax) {
      xmax = data[i];
    }
  }
  return xmax;
}

double min(vector<double> data) {
  double xmin = data[0];
  for (int i = 1; i < data.size(); i++) {
    if (data[i] < xmin) {
      xmin = data[i];
    }
  }
  return xmin;
}

int max(int x1, int x2) {
  if (x1 > x2) {
    return x1;
  } else {
    return x2;
  }
}

int min(int x1, int x2) {
  if (x1 < x2) {
    return x1;
  } else {
    return x2;
  }
}

double max(double x1, double x2) {
  if (x1 > x2) {
    return x1;
  } else {
    return x2;
  }
}

double min(double x1, double x2) {
  if (x1 < x2) {
    return x1;
  } else {
    return x2;
  }
}

double map(double x, double in_min, double in_max, double out_min,
           double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

vector<double> resample(vector<double> xdata, vector<double> ydata,
                        int newlength) {
  int oldlength = ydata.size();
  vector<double> newdata(newlength);
  double x, y, x1, y1, x2, y2;

  if (oldlength == newlength) {
    return ydata;
  } else {
    for (int newindex = 0; newindex < newlength; newindex++) {
      x = (double)newindex / ((double)newlength - 1.0) *
              (xdata.back() - xdata.front()) +
          xdata.front();
      for (size_t ii = 0; ii < xdata.size() - 1; ii++)
        if (x >= xdata[ii] and x <= xdata[ii + 1]) // Determine bin
        {
          x1 = xdata[ii];
          x2 = xdata[ii + 1];
          y1 = ydata[ii];
          y2 = ydata[ii + 1];
          y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
          newdata[min(max(0, newindex), newlength - 1)] = y;
          break;
        }
    }
    newdata[0] = ydata[0];
    newdata[newlength - 1] = ydata[oldlength - 1];

    return newdata;
  }
}

AsciiPlotter::AsciiPlotter() {
  _title = "";
  _width = 100;
  _height = 50;
  _curves = 0;
}

AsciiPlotter::AsciiPlotter(string title) {
  _title = title;
  _width = 100;
  _height = 50;
  _curves = 0;
}

AsciiPlotter::AsciiPlotter(string title, int width, int height) {
  _title = title;
  _width = width;
  _height = height;
  _curves = 0;
}

void AsciiPlotter::addPlot(vector<double> xdata, vector<double> ydata,
                           string label = "", char marker = ' ') {
  _xdata[_curves] = xdata;
  _markers[_curves] = marker;
  _labels[_curves] = label;
  _ydata[_curves++] = ydata;
}

void AsciiPlotter::show() {
  std::stringstream ss;
  AsciiPlotter::show(ss);
  cout << ss.str();
}

void AsciiPlotter::show(std::stringstream &ss) {

  double xmin = _xdata[0].front();
  double xmax = _xdata[0].back();

  for (size_t curve = 0; curve < _curves; curve++) {
    xmin = min(xmin, _xdata[curve].front());
    xmax = max(xmax, _xdata[curve].back());
  }

  double ymax = 1.0e-15;
  double ymin = 1.0e15;

  vector<double> resampled;
  vector<vector<char>> plane;

  int padding;
  string lmargin = "          ";

  plane.resize(_width);
  for (int i = 0; i < _width; i++) {
    plane[i].resize(_height);
    for (int j = 0; j < _height; j++) {
      plane[i][j] = ' ';
    }
  }

  for (int curve = 0; curve < _curves; curve++) {
    double mx = max(_ydata[curve]);
    double mn = min(_ydata[curve]);

    if (mx > ymax) {
      ymax = mx;
    }

    if (mn < ymin) {
      ymin = mn;
    }
  }

  for (int curve = 0; curve < _curves; curve++) {
    int nBefore =
        floor(_width * (_xdata[curve].front() - xmin) / (xmax - xmin));
    int nAfter = floor(_width * (xmax - _xdata[curve].back()) / (xmax - xmin));
    resampled =
        resample(_xdata[curve], _ydata[curve], _width - nBefore - nAfter);
    for (int row = 0; row < resampled.size(); row++) {
      int colindex = (int)map(resampled[row], ymin, ymax, 0.0, (double)_height);
      plane[nBefore + row][min(max(0, colindex), _height - 1)] =
          _markers[curve];
    }
  }

  // title:

  ss << endl << endl;

  for (int i = 0; i < lmargin.length() + (_width - _title.length()) / 2 - 1;
       i++) {
    ss << " ";
  }

  ss << _title << endl << endl;

  // main plot plane:

  ss << std::setfill(' ') << std::setw(9) << std::setprecision(3) << ymax
     << " +";

  // sprintf(ss, " %8.2g +", ymax);

  for (int row = 0; row < _width; row++) {
    ss << "-";
  }
  ss << "+" << endl;

  for (int col = _height - 1; col >= 0; col--) {
    if (col == _height / 2) {
      padding = lmargin.length() - _ylabel.length();
      if (padding >= 0) {
        int totwidth = 0;
        for (int i = 0; i < padding - 1; i++) {
          ss << " ";
          totwidth++;
        }
        ss << _ylabel;
        for (int i = totwidth; i < lmargin.length() - _ylabel.length(); i++) {
          ss << " ";
        }
        ss << "|";
      } else {
      }
    } else {
      ss << lmargin << "|";
    }
    for (int row = 0; row < _width; row++) {
      ss << plane[row][col];
    }
    ss << "|" << endl;
  }

  // printf(" %8.2g +", ymin);
  ss << std::setfill(' ') << std::setw(9) << std::setprecision(3) << ymin
     << " +";
  for (int row = 0; row < _width; row++) {
    ss << "-";
  }
  ss << "+" << endl;

  ss << lmargin;
  // printf("%-8.2g", xmin);
  ss << xmin;

  int buf = (_width - _xlabel.length()) / 2;
  for (int row = 0; row < buf; row++) {
    ss << " ";
  }

  ss << _xlabel;

  for (int row = 0; row < buf; row++) {
    ss << " ";
  }

  // printf("%8g", xmax);
  ss << xmax;

  ss << endl << endl;

  // legend:

  if (_legend) {
    ss << lmargin << "+";
    for (int row = 0; row < _width; row++) {
      ss << "-";
    }
    ss << "+" << endl;

    for (int curve = 0; curve < _curves; curve++) {
      ss << lmargin << "|   " << _markers[curve] << " " << _labels[curve];

      for (int i = 0; i < (_width - _labels[curve].length() - 5); i++) {
        ss << " ";
      }
      ss << "|" << endl;
    }

    ss << lmargin << "+";
    for (int row = 0; row < _width; row++) {
      ss << "-";
    }
    ss << "+" << endl;
  }
}

void AsciiPlotter::xlabel(string label) { _xlabel = label; }

void AsciiPlotter::ylabel(string label) { _ylabel = label; }

void AsciiPlotter::legend() { _legend = true; }

void AsciiPlotter::example() {
  _width = 120;
  _height = 20;

  vector<double> t(101);
  vector<double> a(101);
  vector<double> b(101);
  vector<double> c(101);
  double A = 10.0;

  double dt = 0.0002;

  for (int i = 0; i < 101; i++) {
    if (i != 0) {
      t[i] = t[i - 1] + dt;
    }
    a[i] = A * sin(6.28 * 60.0 * t[i]);
    b[i] = A * sin(6.28 * 60.0 * t[i] + 2.09);
    c[i] = A * sin(6.28 * 60.0 * t[i] + 4.19);
  }

  _title = "Three Phase Currents";
  _height = 20;

  AsciiPlotter::addPlot(t, a, "Ia", '.');
  AsciiPlotter::addPlot(t, b, "Ib", 'x');
  AsciiPlotter::addPlot(t, c, "Ic", '*');
  AsciiPlotter::legend();
  AsciiPlotter::xlabel("t (s)");
  AsciiPlotter::ylabel("I (A)");
  AsciiPlotter::show();

  vector<double> x(101);
  vector<double> f(101);

  dt = 0.05;

  for (int i = 0; i < 101; i++) {
    if (i != 0) {
      t[i] = t[i - 1] + dt;
    }

    x[i] = sin(6.28 * t[i] - 2.0 * sin(6.28 * 0.2 * t[i]));
  }

  _curves = 0;
  _title = "FM Signal";

  AsciiPlotter::addPlot(t, x, "Signal", 'x');

  AsciiPlotter::legend();
  AsciiPlotter::xlabel("t (s)");
  AsciiPlotter::ylabel("");
  AsciiPlotter::show();

  vector<double> x1(101);
  vector<double> x2(101);

  dt = 0.2;

  for (int i = 0; i < 101; i++) {
    if (i != 0) {
      t[i] = t[i - 1] + dt;
    }
    x1[i] = sin(t[i]) * exp(-t[i] / 10);
    x2[i] = cos(t[i]) * exp(-t[i] / 10);
  }

  _curves = 0;
  _title = "Dampened Sine Waves with Envelope";

  AsciiPlotter::addPlot(t, x1, "sin(t) * exp(-t/10)", '*');
  AsciiPlotter::addPlot(t, x2, "cos(t) * exp(-t/10)", 'x');

  AsciiPlotter::legend();
  AsciiPlotter::xlabel("t (s)");
  AsciiPlotter::ylabel("");
  AsciiPlotter::show();
}

AsciiPlotter::~AsciiPlotter() {
  // do nothing
}
