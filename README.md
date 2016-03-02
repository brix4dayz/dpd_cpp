# _dpd2_
##### _Copyright (c) 2016, Yingling labs_
##### author: _Hayden Fuss_

#### _Description_
A C++ framework designed for building and, more importantly, analyzing **DPD** simulations. The framework establishes an interface specifically designed for easily solving the clustering problem that is typically associated with colloidal systems. Although the framework allows for multiple cluster solving algoritms, the **BinBox** algorithm is the only one currently implemented.  

A Python wrapper module has also been made so that the **BinBox** solver can be used within Python. The remaining portion of the framework will be implemented in Python so it is easier to extend and use rather than potentially requiring clients to interact directly with the DLL through `ctypes`.  

<div style="text-align:center" markdown="1">

<img src="hd_micelles.jp2" height=650>

</div>
