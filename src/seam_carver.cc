#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

int SeamCarver::GetEnergy(int row, int col) const { 
  int col_change = 0;
  int row_change = 0;
   if (col == 0) {
    int red_col_change = image_.GetPixel(row, width_ - 1).GetRed() - image_.GetPixel(row, col + 1).GetRed();
    int blue_col_change = image_.GetPixel(row, width_ - 1).GetBlue() - image_.GetPixel(row, col + 1).GetBlue();
    int green_col_change = image_.GetPixel(row, width_ - 1).GetGreen() - image_.GetPixel(row, col + 1).GetGreen();
    col_change = (red_col_change*red_col_change) + (blue_col_change*blue_col_change) + (green_col_change*green_col_change);
  } else if (col == width_ - 1) {
    int red_col_change = image_.GetPixel(row, col - 1).GetRed() - image_.GetPixel(row, 0).GetRed();
    int blue_col_change = image_.GetPixel(row, col - 1).GetBlue() - image_.GetPixel(row, 0).GetBlue();
    int green_col_change = image_.GetPixel(row, col - 1).GetGreen() - image_.GetPixel(row, 0).GetGreen();
    col_change = (red_col_change*red_col_change) + (blue_col_change*blue_col_change) + (green_col_change*green_col_change);
  } else {
    int red_col_change = image_.GetPixel(row, col - 1).GetRed() - image_.GetPixel(row, col + 1).GetRed();
    int blue_col_change = image_.GetPixel(row, col - 1).GetBlue() - image_.GetPixel(row, col + 1).GetBlue();
    int green_col_change = image_.GetPixel(row, col - 1).GetGreen() - image_.GetPixel(row, col + 1).GetGreen();
    col_change = (red_col_change*red_col_change) + (blue_col_change*blue_col_change) + (green_col_change*green_col_change);
  }
   if (row == 0) {
    int red_row_change = image_.GetPixel(height_ - 1, col).GetRed() - image_.GetPixel(row + 1, col).GetRed();
    int blue_row_change = image_.GetPixel(height_ - 1, col).GetBlue() - image_.GetPixel(row + 1, col).GetBlue();
    int green_row_change = image_.GetPixel(height_ - 1, col).GetGreen() - image_.GetPixel(row + 1, col).GetGreen();
    row_change = (red_row_change*red_row_change) + (blue_row_change*blue_row_change) + (green_row_change*green_row_change);
  } else if (row == height_ - 1) {
    int red_row_change = image_.GetPixel(0, col).GetRed() - image_.GetPixel(row - 1, col).GetRed();
    int blue_row_change = image_.GetPixel(0, col).GetBlue() - image_.GetPixel(row - 1, col).GetBlue();
    int green_row_change = image_.GetPixel(0, col).GetGreen() - image_.GetPixel(row - 1, col).GetGreen();
    row_change = (red_row_change*red_row_change) + (blue_row_change*blue_row_change) + (green_row_change*green_row_change);
  } else {
    int red_row_change = image_.GetPixel(row - 1, col).GetRed() - image_.GetPixel(row + 1, col).GetRed();
    int blue_row_change = image_.GetPixel(row - 1, col).GetBlue() - image_.GetPixel(row + 1, col).GetBlue();
    int green_row_change = image_.GetPixel(row - 1, col).GetGreen() - image_.GetPixel(row + 1, col).GetGreen();
    row_change = (red_row_change*red_row_change) + (blue_row_change*blue_row_change) + (green_row_change*green_row_change);
  }
  int energy = col_change + row_change;
  return energy;
}

int* SeamCarver::GetHorizontalSeam() const {
  int** values = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    values[row] = new int[width_];
  }
  
  for (int row = 0; row < height_; ++row) {
     values[row][width_ - 1] = GetEnergy(row, width_ - 1);
  }

  FindHorizontalSeam(values);

  int* path_rows = new int[width_];
  int min_left_value = values[0][0];
  int min_left_row = 0;
  for (int row = 1; row < height_; ++row) {
    if (values[row][0] < min_left_value) {
      min_left_value = values[row][0];
      min_left_row = row;
    }
  }
  path_rows[0] = min_left_row;
  
  TraceSeamHorizontal(path_rows, values);

  for (int row = 0; row < height_; ++row) {
    delete[] values[row];
  }
  delete[] values;
  values = nullptr;

  return path_rows;
}

void SeamCarver::FindHorizontalSeam(int** values) const {
  for (int col = width_ - 2; col >= 0; --col) {
    for (int row = 0; row < height_; ++row) {
      int best = values[row][col + 1];
      if (row != 0) {
        int up_right = values[row - 1][col + 1];
        if (up_right < best) {
          best = up_right;
        }
      }
      if (row != height_ - 1) {
        int down_right = values[row + 1][col + 1];
        if (down_right < best) {
          best = down_right;
        }
      }
      values[row][col] = GetEnergy(row, col) + best;
    }
  }
}

void SeamCarver::TraceSeamHorizontal(int* &path_rows, int** values) const {
  for (int col = 1; col < width_; ++col) {
    int prev_row = path_rows[col - 1];
    int min_value = values[prev_row][col];
    int min_row = prev_row;
    if (prev_row != 0) {
      int up = values[prev_row - 1][col];
      if (up < min_value) {
        min_value = up;
        min_row = prev_row - 1;
      }
    }
    if (prev_row != height_ - 1) {
      int down = values[prev_row + 1][col];
      if (down < min_value) {
        min_value = down;
        min_row = prev_row + 1;
      }
    }
    path_rows[col] = min_row;
  }
}


int* SeamCarver::GetVerticalSeam() const {
  int** values = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    values[row] = new int[width_];
  }
  
  for (int col = 0; col < width_; ++col) {
     values[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }

  FindVerticalSeam(values);

  int* path_cols = new int[height_];
  int min_top_value = values[0][0];
  int min_top_col = 0;
  for (int col = 0; col < width_; ++col) {
    if (values[0][col] < min_top_value) {
      min_top_value = values[0][col];
      min_top_col = col;
    }
  }
  path_cols[0] = min_top_col;
  
  TraceSeamVertical(path_cols, values);

  for (int row = 0; row < height_; ++row) {
    delete[] values[row];
  }
  delete[] values;
  values = nullptr;

  return path_cols;
}
void SeamCarver::FindVerticalSeam(int** values) const {
  for (int row = height_ - 2; row >= 0; --row) {
    for (int col = 0; col < width_; ++col) {
      int best = values[row + 1][col];
      if (col != 0) {
        int left_down = values[row + 1][col - 1];
        if (left_down < best) {
          best = left_down;
        }
      }
      if (col != width_ - 1) {
        int right_down = values[row + 1][col + 1];
        if (right_down < best) {
          best = right_down;
        }
      }
      values[row][col] = GetEnergy(row, col) + best;
    }
  }
}

void SeamCarver::TraceSeamVertical(int* &path_cols, int** values) const {
  for (int row = 1; row < height_; ++row) {
    int prev_col = path_cols[row - 1];
    int min_value = values[row][prev_col];
    int min_col = prev_col;
    if (prev_col != 0) {
      int left = values[row][prev_col - 1];
      if (left < min_value) {
        min_value = left;
        min_col = prev_col - 1;
      }
    }
    if (prev_col != width_ - 1) {
      int right = values[row][prev_col + 1];
      if (right < min_value) {
        min_value = right;
        min_col = prev_col + 1;
      }
    }
    path_cols[row] = min_col;
  }
}

void SeamCarver::RemoveHorizontalSeam() {
  int* path_rows = GetHorizontalSeam();
  image_.RemoveHorizontalSeam(path_rows);
  height_ = height_ - 1;
  delete[] path_rows;
  path_rows = nullptr;
}

void SeamCarver::RemoveVerticalSeam() {
  int* path_cols = GetVerticalSeam();
  image_.RemoveVerticalSeam(path_cols);
  width_ = width_ - 1;
  delete[] path_cols;
  path_cols = nullptr;
}


