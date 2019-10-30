#include <windows.h>
#include <gl/gl.h>
#include "SOIL.h"

/*
Grupo ALR
Integrantes
Arthur Fernandes Ribeiro Costa  Matrícula: 11911BCC059
Loredana Romano Devico          Matrícula: 11911BCC001
Robson Roberto Vieira Junior    Matrícula: 11911BCC056

*/


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WindowProc2(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);
int dificuldade,cc=0,start=0;

GLuint tx_Inimigo[3],tx_Canhao[3],tx_Tiro,tx_Morte[10],tx_Fundo[7],tx_score[10];
int pontos=0;
int milhares, centenas,dezenas,unidades;
//====textura====//
char str[50];
static GLuint carregaArqTextura(char *str);
void carregaTexturas();
//=====
int anima;
float dif=1;

//======= Canhão=====//
int status_canhao=1,c_para;
int move_canhao = 0;
double v_canhao=0;

//=====Inimigo====//
int d_inimigo=1,total_inimigos=27;
double v_inimigo=0,x1_tiro,x2_tiro;
double y=0.95;
int status_inimigo[3][9];

//====tiro====//
int t_status=0,tem_x,t_subiu=0;
float x1,x2,coord1,coord2,v_tiro=0,t_mata=1,t_matou;





int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;


    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          700,
                          700,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);


   TodosInimigos();//roda 1 vez e deixa todos os inimigos vivos
    carregaTexturas();



    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            TelaInicial();

    if(start==2)
        TelaDificuldade();
    if(start==3)
        TelaCreditos();
    if(start==-1)
        TelaDS();

              if(status_canhao ==1 && total_inimigos!=0 && start==1){/* canhão vivo && inimigos < qt total){ */
            glClear(GL_COLOR_BUFFER_BIT);
                TelaJogo(); //fundo
                Desenha_inimigos();// inimigos
                Desenha_tiro(); //tiro
                subir_tiro();
                Desenha_Canhao();// nave
                score();
                }
                else
                {
                    if(total_inimigos==0)
                    {
                         TelaWin(); // tela de ganhou
                          score();
                    }

                    else
                        if(status_canhao==0)
                        {
                             TelaLost();//tela de perdeu
                             score();
                        }

                }
            glPopMatrix();
            SwapBuffers(hDC);
            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
                case VK_RIGHT:
                    {
                        cc=1;
                        move_canhao=1;
                        break;
                    }
                case VK_LEFT:
                    {
                        cc=2;
                        move_canhao=-1;
                        break;
                    }
                case VK_SPACE:
                    case VK_UP:
                    {
                        if(t_status==0)
                            {
                            t_status=1;
                            }
                break;
                    }

                case VK_NUMPAD2:
                        case '2':
                            {
                           start=2;
                           break;
                        //dificuldade
                            case VK_NUMPAD5:
                        case '5'://facil
                            {
                             dif=0.1;
                            start=1;
                            break;
                            }
                            case VK_NUMPAD6:
                        case '6'://normal
                            {
                            dif=1;
                            start=1;
                            break;
                            }
                            case VK_NUMPAD7:
                        case '7'://Dificil
                            {
                            dif=2;
                            start=1;
                            break;
                            }
                            case VK_NUMPAD8:
                        case '8'://DK
                            start=-1;
                            break;
                         }

                case VK_NUMPAD3:
                        case '3':
                            start=3;
                        break;

                case VK_NUMPAD1:
                        case '1':
                            start=1;
                        break;
            }
        }
        break;
                case WM_KEYUP:
                    switch (wParam)
            {
                case VK_RIGHT:
                    {
                        cc=0;
                        move_canhao=0;
                        break;
                    }

                case VK_LEFT:
                    {
                        cc=0;
                        move_canhao=0;
                        break;
                    }

            }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}


void Desenha_Canhao()
{

    if(move_canhao==1){

            if(v_canhao>= 0.9){
            v_canhao=v_canhao; //canhão não sai da tela


            }
            else{
        v_canhao+=0.0025;

        }
    }
    if(move_canhao==-1)
    {

       if(v_canhao<= -0.9){

        v_canhao=v_canhao;//canhão não sai da tela
       }

        else{

            v_canhao-=0.0025;
            }
        }

        glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Canhao[cc]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(-0.04+v_canhao, -0.65);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(0.06+v_canhao, -0.65);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(0.06+v_canhao, -0.75);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(-0.04+v_canhao, -0.75);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);

}

void TodosInimigos(){
    int aux,aux2;
    for(aux=0; aux<3; aux++)
        for(aux2=0; aux2<9; aux2++)
            status_inimigo[aux][aux2]=1;}

void Desenha_inimigos()
{
    double x;
    double xp=-0.75,xn=-0.85;
    float xinip=xp+v_inimigo;
    float xinin=xn+v_inimigo;
    int x2;


    if (d_inimigo== 1)
        v_inimigo+=0.00025*dif;// velocidade base * dificuldade do jogo
    if (d_inimigo== -1)
        v_inimigo-=0.00025*dif;

if (xinip +1.6 >= 1){
        y-=0.02;//quanto o inimigo desce
        d_inimigo= -1;}
    if (xinin <= -1){
        y-=0.02;
        d_inimigo= 1;
    }




// Inimigo 1=================
       for (x=0;x<9;x++){
x2=x;
       if (status_inimigo[0][x2]>=1)
        {
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Inimigo[0]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(xinip+(x/5),  y);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(xinin+(x/5),  y);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(xinin+(x/5),  y-0.1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(xinip+(x/5),  y-0.1);

//colisão
               if(-0.65+v_tiro >= y-0.1 && -0.65+v_tiro <=y) //y1_tiro >=y3_ini  && y2_tiro >=y2_ini
                    if((xinip+(x/5)-0.05) <= coord1+0.02 || (xinip+(x/5)-0.05) <= coord2-0.02)// (x4_ini <= x1_tiro || (x4_ini <= x2_tiro)
                        if(coord1+0.02 <=(xinin+(x/5)-0.05) || coord1+0.02 <= (xinip+(x/5)-0.05)) //( && x1_tiro <=x3_inimigo) x1_tiro <= x4_ini) &&
                            if(t_mata==1)
                            {
                             status_inimigo[0][x2]=0;
                             t_matou=1;
                             t_mata=0;
                             total_inimigos--;
                            pontos+=50*dif;
                            }

            if(y-0.1 <= -0.65)
                    status_canhao=0;


                    glEnd();
                    glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);

       }
       else if(status_inimigo[0][x2]<=0 && status_inimigo[0][x2]>=-100)
       {


            // animação de morte
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);
    if(status_inimigo[0][x2]<=0 && status_inimigo[0][x2]>=-10)
    anima=0;

    if(status_inimigo[0][x2]<-10 && status_inimigo[0][x2]>=-20)
    anima=1;

    if(status_inimigo[0][x2]<-20 && status_inimigo[0][x2]>=-30)
    anima=2;

    if(status_inimigo[0][x2]<-30 && status_inimigo[0][x2]>=-40)
    anima=3;

    if(status_inimigo[0][x2]<-40 && status_inimigo[0][x2]>=-50)
    anima=4;

    if(status_inimigo[0][x2]<-50 && status_inimigo[0][x2]>=-60)
    anima=5;

    if(status_inimigo[0][x2]<-60 && status_inimigo[0][x2]>=-70)
    anima=6;

    if(status_inimigo[0][x2]<-70 && status_inimigo[0][x2]>=-80)
    anima=7;

    if(status_inimigo[0][x2]<-80 && status_inimigo[0][x2]>=-90)
    anima=8;

    if(status_inimigo[0][x2]<-90 && status_inimigo[0][x2]>=-100)
    anima=9;

    glBindTexture(GL_TEXTURE_2D, tx_Morte[anima]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(xinip+(x/5),  y);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(xinin+(x/5),  y);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(xinin+(x/5),  y-0.1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(xinip+(x/5),  y-0.1);

                        glEnd();
                    glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            status_inimigo[0][x2]--;
            }






// Inimigo 2 ================

            if (status_inimigo[1][x2]==1)
        {

            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Inimigo[1]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(xinip+(x/5),  y-0.15);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(xinin+(x/5),  y-0.15);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(xinin+(x/5),  y-0.25);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(xinip+(x/5),  y-0.25);
        //colisão
                    if(-0.65+v_tiro >= y-0.25 && -0.65+v_tiro <=y-0.15) //y1_tiro >=y3_ini  && y2_tiro >=y2_ini
                    if((xinip+(x/5)-0.05) <= coord1+0.02  || (xinip+(x/5)-0.05) <= coord2-0.02 )// (x4_ini <= x1_tiro || (x4_ini <= x2_tiro)
                        if(coord1+0.02 <=(xinin+(x/5)-0.05) || coord1+0.02 <= (xinip+(x/5)-0.05)) //( && x1_tiro <=x3_inimigo) x1_tiro <= x4_ini) &&
                            if(t_mata==1)
                            {
                             status_inimigo[1][x2]=0;
                             t_matou=1;
                             t_mata=0;
                             total_inimigos--;
                              pontos+=30*dif;//  printf("%i\n",pontos);
                            }

            if(y-0.25 <= -0.65)
                status_canhao=0;

                glEnd();
                glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);

        }else if(status_inimigo[1][x2]<=0 && status_inimigo[1][x2]>=-100)
       {
        anima=0;

            // animação de morte
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);
    if(status_inimigo[1][x2]<=0 && status_inimigo[1][x2]>=-10)
    anima=0;

    if(status_inimigo[1][x2]<-10 && status_inimigo[1][x2]>=-20)
    anima=1;

    if(status_inimigo[1][x2]<-20 && status_inimigo[1][x2]>=-30)
    anima=2;

    if(status_inimigo[1][x2]<-30 && status_inimigo[1][x2]>=-40)
    anima=3;

    if(status_inimigo[1][x2]<-40 && status_inimigo[1][x2]>=-50)
    anima=4;

    if(status_inimigo[1][x2]<-50 && status_inimigo[1][x2]>=-60)
    anima=5;

    if(status_inimigo[1][x2]<-60 && status_inimigo[1][x2]>=-70)
    anima=6;

    if(status_inimigo[1][x2]<-70 && status_inimigo[1][x2]>=-80)
    anima=7;

    if(status_inimigo[1][x2]<-80 && status_inimigo[1][x2]>=-90)
    anima=8;

    if(status_inimigo[1][x2]<-90 && status_inimigo[1][x2]>=-100)
    anima=9;

    glBindTexture(GL_TEXTURE_2D, tx_Morte[anima]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(xinip+(x/5),  y-0.15);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(xinin+(x/5),  y-0.15);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(xinin+(x/5),  y-0.25);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(xinip+(x/5),  y-0.25);

                        glEnd();
                    glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            status_inimigo[1][x2]--;
            }


// inimigo 3 ==============
if (status_inimigo[2][x2]==1)
        {
                  glPushMatrix();
            glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Inimigo[2]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(xinip+(x/5),  y-0.30);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(xinin+(x/5),  y-0.30);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(xinin+(x/5),  y-0.40);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(xinip+(x/5),  y-0.40);
                    //colisão
                    if(-0.65+v_tiro >= y-0.4 && -0.65+v_tiro <=y-0.3) //y1_tiro >=y3_ini  && y1_tiro >=y2_ini
                    if((xinip+(x/5)-0.05) <= coord1+0.02  || (xinip+(x/5)-0.05) <= coord2-0.02 )// (x4_ini <= x1_tiro || (x4_ini <= x2_tiro)
                        if(coord1+0.02  <=(xinin+(x/5)-0.05) || coord1+0.02 <= (xinip+(x/5)-0.05) )//( && x1_tiro <=x3_inimigo) x1_tiro <= x4_ini) &&
                            if(t_mata==1)
                            {
                             status_inimigo[2][x2]=0;
                             t_matou=1;
                             t_mata=0;
                             total_inimigos--;
                              pontos+=10*dif;

                            }

            if(y-0.40 <= -0.65)
                status_canhao=0;

                glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }else if(status_inimigo[2][x2]<=0 && status_inimigo[2][x2]>=-100)
       {
        anima=0;

        // animação de morte
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);
    if(status_inimigo[2][x2]<=0 && status_inimigo[2][x2]>=-10)
    anima=0;

    if(status_inimigo[2][x2]<-100 && status_inimigo[2][x2]>=-20)
    anima=1;

    if(status_inimigo[2][x2]<-20 && status_inimigo[2][x2]>=-30)
    anima=2;

    if(status_inimigo[2][x2]<-30 && status_inimigo[2][x2]>=-40)
    anima=3;

    if(status_inimigo[2][x2]<-40 && status_inimigo[2][x2]>=-50)
    anima=4;

    if(status_inimigo[2][x2]<-50 && status_inimigo[2][x2]>=-60)
    anima=5;

    if(status_inimigo[2][x2]<-60 && status_inimigo[2][x2]>=-70)
    anima=6;

    if(status_inimigo[2][x2]<-70 && status_inimigo[2][x2]>=-80)
    anima=7;

    if(status_inimigo[2][x2]<-80 && status_inimigo[2][x2]>=-90)
    anima=8;

    if(status_inimigo[2][x2]<-90 && status_inimigo[2][x2]>=-100)
    anima=9;

    glBindTexture(GL_TEXTURE_2D, tx_Morte[anima]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(xinip+(x/5),  y-0.30);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(xinin+(x/5),  y-0.30);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(xinin+(x/5),  y-0.40);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(xinip+(x/5),  y-0.40);

                        glEnd();
                    glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            status_inimigo[2][x2]--;
            }

        }// fim dos for
}

void Desenha_tiro()
{
        if(tem_x==0)
        {
            coord1=-0.04+v_canhao;
            coord2=0.06+v_canhao;
        }
        else
        {
            coord1=x1;
            coord2=x2;
        }
if(t_matou==0)
    {
     glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Tiro);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(coord1, -0.65+v_tiro);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(coord2, -0.65+v_tiro);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(coord2, -0.75+v_tiro);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(coord1, -0.75+v_tiro);

                glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
        }
}

void subir_tiro()
{
        if(t_status==1 ) // tem tiro
        {
            if(t_subiu==0)//não subiu ainda
            {
            x1=-0.04+v_canhao; //printf("\n%f\n",x1);
            x2=0.06+v_canhao; //printf("%f\n",x2);
            tem_x=1;
            }
            if(v_tiro-0.79<=1 && t_matou==0) //tiro não chegou no teto ou  não matou
                {

                    t_subiu=1;
                    v_tiro+=0.0045;
                }
                    else
                       // if(t_matou == 1)
                            {
                            t_matou=0;
                            t_mata=1;
                            t_status=0;
                            t_subiu=0;
                            v_tiro=0;
                            tem_x=0;
                            }


                }
            else
               {
                t_status=0;
                t_subiu=0;
                v_tiro=0;
                tem_x=0;
               }
        }



void carregaTexturas()
{
    int c;

    for(c=0;c<7;c++)
    {
    sprintf(str,".//Sprites//Telas//fundo%i.png",c);
    tx_Fundo[c] = carregaArqTextura(str);
    }

    for(c=0;c<10;c++)
    {
    sprintf(str,".//Sprites//Score//%i.png",c);
    tx_score[c] = carregaArqTextura(str);
    }

    for(c=0;c<10;c++)
    {
    sprintf(str,".//Sprites//Morte//explosao%d.png",c);
    tx_Morte[c] = carregaArqTextura(str);
    }

    for(c=0;c<3;c++)
    {
    sprintf(str,".//Sprites//canhao%i.png",c);
    tx_Canhao[c] = carregaArqTextura(str);
    }

    sprintf(str,".//Sprites//tiro.png");
    tx_Tiro = carregaArqTextura(str);

    for(c=0;c<3;c++)
    {
       sprintf(str,".//Sprites//inimigo_%d.png",c);
        tx_Inimigo[c] = carregaArqTextura(str);
    }


}
static GLuint carregaArqTextura(char *str){




    // http://www.lonesock.net/soil.html
    GLuint tex = SOIL_load_OGL_texture
        (
            str,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y |
            SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );

    /* check for an error during the load process */
    if(0 == tex){
        printf( "SOIL loading error:");
    }

    return tex;
}

void TelaInicial()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Fundo[0]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(-1, 1);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(-1, -1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(1, -1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);


}
void TelaJogo() // tela fundo
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Fundo[2]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(-1, 1);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(-1, -1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(1, -1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);


}

void TelaWin()
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Fundo[5]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(-1, 1);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(-1, -1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(1, -1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);

}
void TelaLost()
{
 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Fundo[4]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(-1, 1);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(-1, -1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(1, -1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
}
void TelaCreditos()
{
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Fundo[1]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(-1, 1);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(-1, -1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(1, -1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
}

void TelaDificuldade()
{
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Fundo[3]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(-1, 1);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(-1, -1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(1, -1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
}
void TelaDS()
{
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glPushMatrix();
        glEnable(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, tx_Fundo[6]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(1, 1);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(-1, 1);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(-1, -1);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(1, -1);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
}

void score()
{
unidades=pontos%10;
dezenas=(pontos%100)/10;
centenas=(pontos%1000)/100;
milhares=(pontos%10000)/1000;

            //======milhares
            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tx_score[milhares]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(0.755, -0.92);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(0.7, -0.92);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(0.7, -0.99);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(0.755, -0.99);
            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            //=======centenas

            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tx_score[centenas]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(0.836, -0.92);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(0.766, -0.92);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(0.766, -0.99);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(0.836, -0.99);

            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            //======dezenas

            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tx_score[dezenas]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(0.914, -0.92);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(0.848, -0.92);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(0.848, -0.99);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(0.914, -0.99);

            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
            //========unidades

            glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tx_score[unidades]);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBegin(GL_QUADS);
                    glTexCoord2f(1.0f,1.0f); glVertex2f(0.994, -0.92);
                    glTexCoord2f(0.0f,1.0f); glVertex2f(0.928, -0.92);
                    glTexCoord2f(0.0f,0.0f); glVertex2f(0.928, -0.99);
                    glTexCoord2f(1.0f,0.0f); glVertex2f(0.994, -0.99);

            glEnd();
            glDisable(GL_BLEND);
            glDisable(GL_TEXTURE_2D);
}



