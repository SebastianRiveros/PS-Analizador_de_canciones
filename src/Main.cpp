#include <windows.h>
#include <string>
#include <sstream>
#include "SistemaRecomendacion.hpp"
using namespace std;

// IDs de controles
enum {
    IDC_BTN_LOAD = 100,
    IDC_EDIT_CSV,
    IDC_BTN_FV,
    IDC_EDIT_FV_IDC,
    IDC_EDIT_FV_N,
    IDC_BTN_TOP,
    IDC_EDIT_TOP_K,
    IDC_BTN_SIM,
    IDC_EDIT_SIM_IDU,
    IDC_EDIT_SIM_K,
    IDC_BTN_REC,
    IDC_EDIT_REC_IDU,
    IDC_EDIT_REC_K,
    IDC_LIST_OUTPUT
};

// instancia global del sistema
static SistemaRecomendacion sistema;

// auxiliar para añadir línea al listbox
void AddLine(HWND hList, const string& line) {
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)line.c_str());
}

// ventana principal
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // edit para ruta CSV (ruta relativa a ../data/)
        CreateWindowEx(
            0, "EDIT", "../data/ratings_s.csv",
            WS_CHILD|WS_VISIBLE|WS_BORDER,
            10, 10, 250, 20,
            hWnd, (HMENU)IDC_EDIT_CSV, nullptr, nullptr
        );
        // botón cargar
        CreateWindow(
            "BUTTON", "Cargar CSV",
            WS_CHILD|WS_VISIBLE,
            270,10,80,20,
            hWnd, (HMENU)IDC_BTN_LOAD, nullptr, nullptr
        );

        // primeros votantes
        CreateWindow("STATIC","Primeros votantes:",WS_CHILD|WS_VISIBLE,
                     10,40,120,15,hWnd,nullptr,nullptr,nullptr);
        CreateWindow("EDIT","1",WS_CHILD|WS_VISIBLE|WS_BORDER,
                     10,60,40,20,hWnd,(HMENU)IDC_EDIT_FV_IDC,nullptr,nullptr);
        CreateWindow("EDIT","5",WS_CHILD|WS_VISIBLE|WS_BORDER,
                     60,60,40,20,hWnd,(HMENU)IDC_EDIT_FV_N,nullptr,nullptr);
        CreateWindow("BUTTON","Ver",WS_CHILD|WS_VISIBLE,
                     110,60,40,20,hWnd,(HMENU)IDC_BTN_FV,nullptr,nullptr);

        // top canciones
        CreateWindow("STATIC","Top K canciones:",WS_CHILD|WS_VISIBLE,
                     10,90,120,15,hWnd,nullptr,nullptr,nullptr);
        CreateWindow("EDIT","10",WS_CHILD|WS_VISIBLE|WS_BORDER,
                     10,110,40,20,hWnd,(HMENU)IDC_EDIT_TOP_K,nullptr,nullptr);
        CreateWindow("BUTTON","Ver",WS_CHILD|WS_VISIBLE,
                     60,110,40,20,hWnd,(HMENU)IDC_BTN_TOP,nullptr,nullptr);

        // usuarios similares
        CreateWindow("STATIC","Usuarios similares:",WS_CHILD|WS_VISIBLE,
                     10,140,120,15,hWnd,nullptr,nullptr,nullptr);
        CreateWindow("EDIT","1",WS_CHILD|WS_VISIBLE|WS_BORDER,
                     10,160,40,20,hWnd,(HMENU)IDC_EDIT_SIM_IDU,nullptr,nullptr);
        CreateWindow("EDIT","5",WS_CHILD|WS_VISIBLE|WS_BORDER,
                     60,160,40,20,hWnd,(HMENU)IDC_EDIT_SIM_K,nullptr,nullptr);
        CreateWindow("BUTTON","Ver",WS_CHILD|WS_VISIBLE,
                     110,160,40,20,hWnd,(HMENU)IDC_BTN_SIM,nullptr,nullptr);

        // recomendar canciones
        CreateWindow("STATIC","Recomendar canciones:",WS_CHILD|WS_VISIBLE,
                     10,190,140,15,hWnd,nullptr,nullptr,nullptr);
        CreateWindow("EDIT","1",WS_CHILD|WS_VISIBLE|WS_BORDER,
                     10,210,40,20,hWnd,(HMENU)IDC_EDIT_REC_IDU,nullptr,nullptr);
        CreateWindow("EDIT","5",WS_CHILD|WS_VISIBLE|WS_BORDER,
                     60,210,40,20,hWnd,(HMENU)IDC_EDIT_REC_K,nullptr,nullptr);
        CreateWindow("BUTTON","Ver",WS_CHILD|WS_VISIBLE,
                     110,210,40,20,hWnd,(HMENU)IDC_BTN_REC,nullptr,nullptr);

        // listbox de salida
        CreateWindow(
            "LISTBOX", nullptr,
            WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL,
            10, 250, 760, 300,
            hWnd, (HMENU)IDC_LIST_OUTPUT, nullptr, nullptr
        );
        break;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        switch (id) {
        case IDC_BTN_LOAD: {
            char buf[260];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_CSV), buf, 260);
            sistema.cargarDatosCSV(buf);
            AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), "CSV cargado.");
            break;
        }
        case IDC_BTN_FV: {
            char bufi[16], bufn[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FV_IDC), bufi, 16);
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FV_N),    bufn, 16);
            int idC = atoi(bufi), n = atoi(bufn);
            auto v = sistema.getFirstVoters(idC, n);
            ostringstream os;
            os << "Primeros " << n << " votantes de " << idC << ":";
            AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), os.str());
            for (int u : v) {
                ostringstream o2; o2 << "  Usuario " << u;
                AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), o2.str());
            }
            break;
        }
        case IDC_BTN_TOP: {
            char bufk[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_TOP_K), bufk, 16);
            int k = atoi(bufk);
            auto top = sistema.getTopSongs(k);
            ostringstream os; os << "Top " << k << ":";
            AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), os.str());
            for (int c : top) {
                ostringstream o2; o2 << "  Canción " << c;
                AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), o2.str());
            }
            break;
        }
        case IDC_BTN_SIM: {
            char bufi[16], bufk[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_SIM_IDU), bufi, 16);
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_SIM_K),    bufk, 16);
            int idU = atoi(bufi), k = atoi(bufk);
            auto sim = sistema.getSimilarUsers(idU, k);
            ostringstream os; os << "Similares a " << idU << ":";
            AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), os.str());
            for (int u : sim) {
                ostringstream o2; o2 << "  Usuario " << u;
                AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), o2.str());
            }
            break;
        }
        case IDC_BTN_REC: {
            char bufi[16], bufk[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_REC_IDU), bufi, 16);
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_REC_K),    bufk, 16);
            int idU = atoi(bufi), k = atoi(bufk);
            auto rec = sistema.recommendSongs(idU, k);
            ostringstream os; os << "Recs para " << idU << ":";
            AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), os.str());
            for (int c : rec) {
                ostringstream o2; o2 << "  Canción " << c;
                AddLine(GetDlgItem(hWnd, IDC_LIST_OUTPUT), o2.str());
            }
            break;
        }
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    // registro de la clase de ventana
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "RecMusClass";
    RegisterClass(&wc);

    // creación de la ventana principal
    HWND hWnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        "Recomendador Musical",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        nullptr, nullptr, hInst, nullptr
    );
    ShowWindow(hWnd, nCmdShow);

    // bucle de mensajes
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
