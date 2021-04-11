#/bin/bash
#
# -------------------Citation----------------
# 
# [1]X.X. Lee et al.Nuclear Techniques,2020,43(08):90-98. 
#            DOI:10.11889/j.0253-3219.2020.hjs.43.080501
# [2]Coming Soon ...
#
# -------------------CopyRight----------------
#
#  Data Analysis Code for 
#      Neutron Capture Cross Section @ Back-n
#                           
# Author  : X.X. Lee
# Email   : lixinx@mail.ustc.edu.cn
# Institute/Group : 
#   Shanghai Laser Electron Gamma Source(SLEGS),CAS
# Date    : 2021.04.10
#
# -------------Parameters---------------------
FileNo="15905"
FileName="Ag-107"
start=101
end=200
# ------------Read Me--------------------------
#
#  FileNo   : File number,daq-[15905]-NORM-00001.root
#  FileName : Sample,BKG,Carborn,Lead ...
#  start    : Number of the 1st File to Process
#  end      : Number of the last File to Process 
#
# --------------------------------------------
PathName=$start"-"$end
PathMain=$(pwd)
cd ${PathMain}/Result/PH
if [ ! -d ${PathName} ]; then
  mkdir ${PathName}
fi
cd ${PathName}
rm -rf ${FileName}
cd ${PathMain}/Result/Out
rm -rf $PathName".root"
cd ${PathMain}/Result/PH
cd ${PathMain}/Code
mkdir PH
./lxx2021 ${start} ${end} ${FileNo}
mv ${PathMain}/Code/PH ../Result/PH/${PathName}/${FileName}
mv ${PathMain}/Code/Out/Data.root ../Result/Out/$PathName".root"
