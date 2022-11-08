#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include <3ds.h>

PrintConsole topScreen, bottomScreen;
char *url;
httpcContext context;
Result ret = 0;

	char buf[256];

Result http_download(httpcContext *context)
{
	ret=0;
	//u8* framebuf_top;
	u32 statuscode=0;
	//u32 size=0;
	u32 contentsize=0;
	u8 *buf;

	ret = httpcBeginRequest(context);
	if(ret!=0)return ret;

	ret = httpcGetResponseStatusCode(context, &statuscode);
	if(ret!=0)return ret;

	//printf("http status code: %i\n", statuscode);

	if(statuscode!=200){
		printf("status code not 200, it was " + statuscode);
		gfxFlushBuffers();
		return -2;
	}

	ret=httpcGetDownloadSizeState(context, NULL, &contentsize);
	if(ret!=0)return ret;
	unsigned char *buffer = (unsigned char*)malloc(contentsize+1);

	consoleSelect(&topScreen);

	printf("HTTP status code: \n" + statuscode);
	printf(" bytes\n" + contentsize);
	gfxFlushBuffers();

	buf = (u8*)malloc(contentsize);
	if(buf==NULL)return -1;
	memset(buf, 0, contentsize);


	ret = httpcDownloadData(context, buffer, contentsize, NULL);
	if(ret!=0)
	{
		free(buf);
		return ret;
	}

	consoleSelect(&bottomScreen);
	printf("%s", buffer);

	free(buf);

	return 0;
}


Result http_downloadsave(httpcContext *context, char *filename)//This error handling needs updated with proper text printing once ctrulib itself supports that.
{
    ret = 0;
	printf("variable ret inicializada");
    //u8* framebuf_top;
    u32 statuscode=0;
	printf("variable statuscode inizializada");
    //u32 size=0;
    u32 contentsize=0;
	printf("variable contentsize inicializada");
    u8 *buf;
	printf("variable buf inicializada");

    ret = httpcBeginRequest(context);
	printf("empezamos request");
    if(ret!=0){printf("lotenemos1\n");return ret;}
	printf("primer if superado!");

    ret = httpcGetResponseStatusCode(context, &statuscode);
	printf("pedimos status code");
    if(ret!=0){printf("lotenemos2\n");return ret;}
	printf("segundo if superado!");

    if(statuscode!=200){
        printf("status code not 200, it was %ld\n", statuscode);
        gfxFlushBuffers();
		printf("flush buffers");
        return -2;
    }

    ret=httpcGetDownloadSizeState(context, NULL, &contentsize);
	printf("get download size state");
    if(ret!=0){printf("lotenemos3\n");return ret;}
	printf("tercer if superado!");
    unsigned char *buffer = (unsigned char*)malloc(contentsize+1);
	printf("inicializamos variable buffer");

    consoleSelect(&topScreen);
	printf("selecionamos la pantalla de arriba");

    printf("HTTP status code: \n" + statuscode);
    printf("bytes\n" + contentsize);
    gfxFlushBuffers();

    buf = (u8*)malloc(contentsize);
    if(buf==NULL){printf("lotenemos4\n");return -1;}
    memset(buf, 0, contentsize);


    ret = httpcDownloadData(context, buffer, contentsize, NULL);
    if(ret!=0)
    {	
		printf("lotenemos5\n");
        free(buf);
        return ret;
    }

	printf("Got file\n");

	//char filename[32];
	FILE *dlfile;

	//snprintf(filename, sizeof(char) * 32, "koopadl%i.txt", ++dlCounter);
	printf("Saving to %s\n", filename);

	dlfile = fopen(filename, "w");
	fwrite(buffer, 1, contentsize, dlfile);
	fclose(dlfile);

	consoleSelect(&topScreen);
	printf("Saved to %s\n", filename);

    //printf("%s", buffer);

    free(buf);

    return 0;
}

void downloadfile()
{
    consoleSelect(&topScreen);

		char *url2 = "https://www.mediafire.com/file/880hzux8wn9rxc5/image.png.gz/file";

		char *file_name;

    file_name = strrchr( url2, '/' ) + 1;

    printf("Downloading %s to %s\n",url2,file_name);
    gfxFlushBuffers();

    ret = httpcOpenContext(&context,HTTPC_METHOD_GET, url2 , 0);
    gfxFlushBuffers();

    if(ret==0)
    {
        ret=http_downloadsave(&context, file_name);
        gfxFlushBuffers();
        httpcCloseContext(&context);
    }
}

int main()
{
	ret = 0;

	gfxInitDefault();

	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleSelect(&topScreen);
	gfxFlushBuffers();

	httpcInit(0);

	url = "https://www.mediafire.com/file/880hzux8wn9rxc5/image.png.gz/file";
	gfxFlushBuffers();

	ret = httpcOpenContext(&context, HTTPC_METHOD_GET, url , 0);
	//printf("return from httpcOpenContext: %"PRId32"\n",ret);
	gfxFlushBuffers();

	if(ret==0)
	{
		ret=http_download(&context);
		//printf("return from http_download: %"PRId32"\n",ret);
		gfxFlushBuffers();
		httpcCloseContext(&context);
	}

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		// Your code goes here
		downloadfile();

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
		break;
	}
}