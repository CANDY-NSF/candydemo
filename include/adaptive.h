#include <pybind11/pybind11.h>



int pageRank(int argc);



namespace py = pybind11;



PYBIND11_MODULE(pybind_11_example, mod) {
    mod.def("pageRank", &pageRank, "Page Rank");
}
