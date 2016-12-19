// Making Integral Images
void CCVFrameDlg::MakeIntegImage(BYTE* src, double* dst, intwidth, int height)
{
	int i,j;
	double partialsum;
	double* tmpdata= newdouble[(width+1)*(height+1)];
	memset(tmpdata,0.0,(width+1)*(height+1));
	for(i=0;i<height+1;i++)
		tmpdata[(width+1)*i+0] = 0;
	for(i=0;i<(width+1);i++)
		tmpdata[(width+1)*0+i] = 0;
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			tmpdata[(width+1)*(i+1)+(j+1)] = src[width*i+j];
		}
	}
	for(i=1;i<(width+1);i++)
	{
		tmpdata[(width+1)*0+i] = tmpdata[(width+1)*0+(i-1)] + tmpdata[(width+1)*0+i];
	}
	for(i=1;i<height+1;i++)
	{
		partialsum = 0;
		for(j=0;j<(width+1);j++)
		{
			partialsum += tmpdata[(width+1)*i+j];
			tmpdata[(width+1)*i+j] = tmpdata[(width+1)*(i-1)+j] + partialsum;
		}
	}
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			dst[width*i+j] = tmpdata[(width+1)*(i+1)+(j+1)];
		}
	}
	delete tmpdata;
}