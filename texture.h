//unsigned char* readBMP(const char* filename, int *height, int *width)
//{
    //int i;
    //FILE* f = fopen(filename, "rb");
    //unsigned char info[54];
    //fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    //// extract image height and width from header
    //*width = *(int*)&info[18];
    //*height = *(int*)&info[22];

    //int size = 3 * (*width) * (*height);
    //unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    //fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    //fclose(f);

    //for(i = 0; i < size; i += 3)
    //{
            //unsigned char tmp = data[i];
            //data[i] = data[i+2];
            //data[i+2] = tmp;
    //}

    //return data;
//}


unsigned char* readBMP(const char* filename, int *height, int *width)
{
	FILE *fd;
	fd = fopen(filename, "rb");

	unsigned char header[54];

	// Read header
	fread(header, sizeof(unsigned char), 54, fd);

	// Capture dimensions
	*width = *(int*)&header[18];
	*height = *(int*)&header[22];

	int padding = 0;

	// Calculate padding
	while ((*width * 3 + padding) % 4 != 0)
	{
		padding++;
	}

	// Compute new width, which includes padding
	int widthnew = *width * 3 + padding;

	// Allocate memory to store image data (non-padded)
	unsigned char* texture = (unsigned char *)malloc(*width * (*height) * 3 * sizeof(unsigned char));

	// Allocate temporary memory to read widthnew size of data
	unsigned char* data = (unsigned char *)malloc(widthnew * sizeof(unsigned int));

	// Read row by row of data and remove padded data.
	for (int i = 0; i < *height; i++)
	{
		// Read widthnew length of data
		fread(data, sizeof(unsigned char), widthnew, fd);

		// Retain width length of data, and swizzle RB component.
		// BMP stores in BGR format, my usecase needs RGB format
		for (int j = 0; j < *width * 3; j += 3)
		{
			int index = (i *  (*width) * 3) + (j);
			texture[index + 0] = data[j + 2];
			texture[index + 1] = data[j + 1];
			texture[index + 2] = data[j + 0];
		}
	}

	free(data);
	fclose(fd);

	return texture;
}

