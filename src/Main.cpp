// src/main.cpp
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
    IDC_BTN_SHOW_SONGS,
    IDC_BTN_SHOW_USERS,
    IDC_EDIT_CLUSTER_K,
    IDC_BTN_CLUSTER,
    IDC_EDIT_BVC_ID,
    IDC_BTN_BVC,
    IDC_EDIT_BVU_ID,
    IDC_BTN_BVU,
    IDC_LIST_OUTPUT
};

// instancia global
static SistemaRecomendacion sistema;

// helper para el listbox
void AddLine(HWND hList, const string& line) {
    SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)line.c_str());
}

// ventana principal
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        // ruta CSV
        CreateWindowEx(0, "EDIT", "../data/ratings_big.csv",
            WS_CHILD|WS_VISIBLE|WS_BORDER, 10,10,250,20,
            hWnd, (HMENU)IDC_EDIT_CSV, nullptr, nullptr);
        CreateWindow("BUTTON","Cargar CSV",
            WS_CHILD|WS_VISIBLE,270,10,80,20,
            hWnd, (HMENU)IDC_BTN_LOAD, nullptr, nullptr);

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

        // mostrar estructuras
        CreateWindow("BUTTON","Mostrar canciones",WS_CHILD|WS_VISIBLE,
            350,10,140,25,hWnd,(HMENU)IDC_BTN_SHOW_SONGS,nullptr,nullptr);
        CreateWindow("BUTTON","Mostrar usuarios",WS_CHILD|WS_VISIBLE,
            500,10,140,25,hWnd,(HMENU)IDC_BTN_SHOW_USERS,nullptr,nullptr);

        // clustering
        CreateWindow("STATIC","Clusters k:",WS_CHILD|WS_VISIBLE,
            350,50,80,15,hWnd,nullptr,nullptr,nullptr);
        CreateWindow("EDIT","2",WS_CHILD|WS_VISIBLE|WS_BORDER,
            350,70,40,20,hWnd,(HMENU)IDC_EDIT_CLUSTER_K,nullptr,nullptr);
        CreateWindow("BUTTON","Cluster",WS_CHILD|WS_VISIBLE,
            400,70,80,20,hWnd,(HMENU)IDC_BTN_CLUSTER,nullptr,nullptr);

        // vals por canción
        CreateWindow("STATIC","Vals p c:",WS_CHILD|WS_VISIBLE,
            350,110,100,15,hWnd,nullptr,nullptr,nullptr);
        CreateWindow("EDIT","1",WS_CHILD|WS_VISIBLE|WS_BORDER,
            350,130,40,20,hWnd,(HMENU)IDC_EDIT_BVC_ID,nullptr,nullptr);
        CreateWindow("BUTTON","Listar",WS_CHILD|WS_VISIBLE,
            400,130,80,20,hWnd,(HMENU)IDC_BTN_BVC,nullptr,nullptr);

        // vals por usuario
        CreateWindow("STATIC","Vals p:",WS_CHILD|WS_VISIBLE,
            350,170,100,15,hWnd,nullptr,nullptr,nullptr);
        CreateWindow("EDIT","1",WS_CHILD|WS_VISIBLE|WS_BORDER,
            350,190,40,20,hWnd,(HMENU)IDC_EDIT_BVU_ID,nullptr,nullptr);
        CreateWindow("BUTTON","Listar",WS_CHILD|WS_VISIBLE,
            400,190,80,20,hWnd,(HMENU)IDC_BTN_BVU,nullptr,nullptr);

        // listbox de salida
        CreateWindow("LISTBOX", nullptr,
            WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL,
            10,250,760,330,
            hWnd, (HMENU)IDC_LIST_OUTPUT, nullptr, nullptr);
        break;
    }

    case WM_COMMAND: {
        int id = LOWORD(wParam);
        HWND hList = GetDlgItem(hWnd, IDC_LIST_OUTPUT);
        switch (id) {
        case IDC_BTN_LOAD: {
            char buf[260];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_CSV), buf, 260);
            sistema.cargarDatosCSV(buf);
            AddLine(hList, "CSV cargado.");
            break;
        }
        case IDC_BTN_FV: {
            char bufi[16], bufn[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FV_IDC), bufi, 16);
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_FV_N), bufn, 16);
            int idC = atoi(bufi), n = atoi(bufn);
            auto v = sistema.getFirstVoters(idC, n);
            AddLine(hList, "-- Primeros votantes --");
            for (int u : v) {
                ostringstream os; os << "Usuario " << u;
                AddLine(hList, os.str());
            }
            break;
        }
        case IDC_BTN_TOP: {
            char bufk[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_TOP_K), bufk, 16);
            int k = atoi(bufk);
            auto top = sistema.getTopSongs(k);
            AddLine(hList, "-- Top canciones --");
            for (int c : top) {
                ostringstream os; os << "Canción " << c;
                AddLine(hList, os.str());
            }
            break;
        }
        case IDC_BTN_SIM: {
            char bufi[16], bufk[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_SIM_IDU), bufi, 16);
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_SIM_K), bufk, 16);
            int idU = atoi(bufi), k = atoi(bufk);
            auto sim = sistema.getSimilarUsers(idU, k);
            AddLine(hList, "-- Usuarios similares --");
            for (int u : sim) {
                ostringstream os; os << "Usuario " << u;
                AddLine(hList, os.str());
            }
            break;
        }
        case IDC_BTN_REC: {
            char bufi[16], bufk[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_REC_IDU), bufi, 16);
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_REC_K), bufk, 16);
            int idU = atoi(bufi), k = atoi(bufk);
            auto rec = sistema.recommendSongs(idU, k);
            AddLine(hList, "-- Recomendaciones --");
            for (int c : rec) {
                ostringstream os; os << "Canción " << c;
                AddLine(hList, os.str());
            }
            break;
        }
        case IDC_BTN_SHOW_SONGS:
            sistema.mostrarCanciones();
            AddLine(hList, "Árbol de canciones mostrado en consola.");
            break;
        case IDC_BTN_SHOW_USERS:
            sistema.mostrarUsuarios();
            AddLine(hList, "Árbol de usuarios mostrado en consola.");
            break;
        case IDC_BTN_CLUSTER: {
            char bufk[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_CLUSTER_K), bufk, 16);
            int k = atoi(bufk);
            auto grupos = sistema.clusterUsuarios(k);
            AddLine(hList, "-- Clusters de usuarios --");
            for (int i = 0; i < (int)grupos.size(); ++i) {
                ostringstream os; os << "Grupo " << (i+1) << ":";
                AddLine(hList, os.str());
                for (int u : grupos[i]) {
                    ostringstream os2; os2 << "  Usuario " << u;
                    AddLine(hList, os2.str());
                }
            }
            break;
        }
        case IDC_BTN_BVC: {
            char bufc[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_BVC_ID), bufc, 16);
            int idC = atoi(bufc);
            auto vals = sistema.buscarValoracionesPorCancion(idC);
            AddLine(hList, "-- Vals por canción --");
            for (auto &p : vals) {
                ostringstream os; os << "Usuario " << p.first << " -> " << p.second;
                AddLine(hList, os.str());
            }
            break;
        }
        case IDC_BTN_BVU: {
            char bufu[16];
            GetWindowText(GetDlgItem(hWnd, IDC_EDIT_BVU_ID), bufu, 16);
            int idU = atoi(bufu);
            auto vals = sistema.buscarValoracionesPorUsuario(idU);
            AddLine(hList, "-- Vals por usuario --");
            for (auto &p : vals) {
                ostringstream os; os << "Canción " << p.first << " -> " << p.second;
                AddLine(hList, os.str());
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
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = "RecMusClass";
    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0, wc.lpszClassName, "Recomendador Musical",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInst, nullptr
    );
    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
