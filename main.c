// video buffer project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <math.h>
#include "GEVA.h"


//#pragma warning(disable : 4996)

void draw_display_buffer(video_buffer* buf) {

    for (uint8_t row = 0; row < buf->size_y; row++) {


            for (uint8_t f = 0; f < buf->byte_col_cnt; f++) {

                for (uint8_t byte_index = 0; byte_index < 8; byte_index++)
                {
                    if (buf->vid_buf[(row * buf->byte_col_cnt) + f] & (0x80 >> byte_index))
                        printf("# ");
                    else
                        printf(". ");
                }
                
            }

            printf("\n");
        }
}

/*
void get_vbc_file(vbc_file* file, const char* s) {

    FILE* vbc = fopen(s, "rb");
    fread(&file->header, sizeof(uint8_t), sizeof(file->header), vbc);
    uint16_t vbc_body_byte = (((uint16_t)file->header.byte_cnt_h) << 8 | file->header.byte_cnt_l);
    //printf("%d\n", vbc_body_byte);
    uint8_t* body = (uint8_t*)malloc(vbc_body_byte);
    fseek(vbc, 5, SEEK_SET);
    fread(body, sizeof(uint8_t), vbc_body_byte, vbc);
    //fclose(vbc);
    file->body = body;
    //return body;
}
*/

video_buffer vid_buf;

int main(int argc, char** argv)
{
    int error = 0;

    vbc_file file_in;
    data_container f;
    data_container l;

    init_buf(&vid_buf, 128, 64);
    set_orientation(LANDSCAPE, &vid_buf);
    set_mode(NORMAL, &vid_buf);


    //put_char(0,0,'h', SMALL, &vid_buf);
    //put_char(0,10,'h', MEDIUM, &vid_buf); 
    put_string(0,8,(uint16_t *)"abcdefghilmnopqrstuvz\n\rjkxyw",SMALL,&vid_buf);
    put_string(0,50,(uint16_t *)"abcdefghilm\n\rnopqrstuvz\n\rjkxwy",MEDIUM,&vid_buf);

    draw_display_buffer(&vid_buf); 
   
    free(vid_buf.vid_buf);
   
    return 0;
}