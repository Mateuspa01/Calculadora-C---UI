#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// Numeros
#define ID_BTN_0 100
#define ID_BTN_1 101
#define ID_BTN_2 102
#define ID_BTN_3 103
#define ID_BTN_4 104
#define ID_BTN_5 105
#define ID_BTN_6 106
#define ID_BTN_7 107
#define ID_BTN_8 108
#define ID_BTN_9 109

// Operações aritméticas    
#define ID_BTN_DIV 110
#define ID_BTN_MUL 111
#define ID_BTN_SUB 112
#define ID_BTN_ADD 113
#define ID_BTN_EQ 114
#define ID_BTN_EC 115

#define ID_EDIT 20

static HWND hEdit, hButtonDiv, hButtonMul, hButtonSub, hButtonAdd, hButtonEq, hButton[10]; 
static HFONT hFont;
static char input[256];
char op = '\0';
double firstNum = 0;
static double secondNum;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Criando caixa de edição
            hEdit = CreateWindow("EDIT", "",
                WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_AUTOHSCROLL | WS_BORDER,
                0, 0, 324, 53,
                hwnd, (HMENU) ID_EDIT, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            
            // Criando os botões

            for(int i = 1; i < 10; i++){
                int x = ((i -1) % 3) * 85;
                int y = 102+(2-(i-1)/3)*50;


                char btnText[2] = { '0' + i, '\0' };
                hButton[i] = CreateWindow("BUTTON", btnText,
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                x, y, 85, 50, hwnd,
                (HMENU) (ID_BTN_0 + i), ((LPCREATESTRUCT)lParam)->hInstance, NULL
                );
            }

            hButton[0] = CreateWindow("BUTTON", "0",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                0, 252, 170, 50, hwnd,
                (HMENU) ID_BTN_0, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
                
            hButtonDiv = CreateWindow("BUTTON", "Limpar",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                0, 52, 255, 50,
                hwnd, (HMENU) ID_BTN_EC, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            //OPERAÇÕES ARITMETICAS    
            hButtonDiv = CreateWindow("BUTTON", "/",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                255, 52, 70, 50,
                hwnd, (HMENU) ID_BTN_DIV, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            
            hButtonMul = CreateWindow("BUTTON", "X",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                255, 102, 70, 50,
                hwnd, (HMENU) ID_BTN_MUL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            
            hButtonSub = CreateWindow("BUTTON", "-",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                255, 152, 70, 50,
                hwnd, (HMENU) ID_BTN_SUB, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            
            hButtonAdd = CreateWindow("BUTTON", "+",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                255, 202, 70, 50,
                hwnd, (HMENU) ID_BTN_ADD, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            
            hButtonEq = CreateWindow("BUTTON", "=",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                170, 252, 155, 50,
                hwnd, (HMENU) ID_BTN_EQ, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            
            input[0] = '\0';
            break;

        case WM_COMMAND:
        if (LOWORD(wParam) >= ID_BTN_0 && LOWORD(wParam) <= ID_BTN_9) {
            char digit = '0' + (LOWORD(wParam) - ID_BTN_0);
            int len = strlen(input);
            if (len < 255) {
                input[len] = digit;
                input[len + 1] = '\0';
                SetWindowText(hEdit, input);
            }
        }else
        if(LOWORD(wParam) == ID_BTN_EQ){
            secondNum = atoi(input);
            double result = 0;
            switch(op){
                case '+': result = firstNum + secondNum; break;
                case '-': result = firstNum - secondNum; break;
                case '*': result = firstNum * secondNum; break;
                case '/': result = (secondNum != 0) ? firstNum / secondNum : 0; break;
            }
            sprintf(input, "%g", result);
            SetWindowText(hEdit, input);
            firstNum = result;
            op = '\0';
        }else
        if(LOWORD(wParam) >= ID_BTN_DIV && LOWORD(wParam) <= ID_BTN_ADD){
            firstNum = atoi(input);
            op = (LOWORD(wParam) == ID_BTN_ADD) ? '+' :
                (LOWORD(wParam) == ID_BTN_SUB) ? '-' :
                (LOWORD(wParam) == ID_BTN_MUL) ? '*' : '/';
            input[0] = '\0';
            SetWindowText(hEdit, "");
        }else
        if(LOWORD(wParam) == ID_BTN_EC){
            input[0] = '\0';
            firstNum = 0;
            op = '\0';
            SetWindowText(hEdit, "");
        }
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine; 
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MinhaJanela";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, "MinhaJanela", "Calculadora", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 340, 340,
        NULL, NULL, hInstance, NULL
    );
    
    if(!hwnd){
        MessageBox(NULL, "Erro ao criar janela!", "Erro", MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}