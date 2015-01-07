cd documentation
doxygen.exe doxygen.project
copy html\index.chm slmath.chm
rd /s /q html
cd ..
php package.php
