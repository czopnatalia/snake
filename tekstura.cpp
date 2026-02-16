/*
* Wczytywanie tekstury
*/

#include "tekstura.h"
GLuint  num_texture=-1;

GLuint WczytajTeksture(const char* filename)
{
  FILE* file;
  fopen_s(&file, filename, "rb");
  if (!file) return 0;

  BITMAPFILEHEADER fileheader;
  BITMAPINFOHEADER infoheader;

  fread(&fileheader, sizeof(fileheader), 1, file);
  fread(&infoheader, sizeof(infoheader), 1, file);

  int width  = infoheader.biWidth;
  int height = infoheader.biHeight;

  int row_padded = (width * 3 + 3) & (~3);   // BMP padding

  unsigned char* data = (unsigned char*)malloc(width * height * 4);
  unsigned char* row = (unsigned char*)malloc(row_padded);

  // BMP czytamy OD DO£U DO GÓRY
  for (int y = 0; y < height; y++)
  {
    fread(row, 1, row_padded, file);

    for (int x = 0; x < width; x++)
    {
      int bmp_i = x * 3;
      int gl_i = ((height - 1 - y) * width + x) * 4;

      data[gl_i + 0] = row[bmp_i + 2];  // R
      data[gl_i + 1] = row[bmp_i + 1];  // G
      data[gl_i + 2] = row[bmp_i + 0];  // B
      data[gl_i + 3] = 255;             // A
    }
  }

  free(row);
  fclose(file);

  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    width,
    height,
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    data
  );

  glGenerateMipmap(GL_TEXTURE_2D);

  free(data);
  return texID;
}


