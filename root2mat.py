from ROOT import *
import numpy as np
import scipy.io
import sys
import os

def GetTH1D(hist):
    Nbinsx = hist.GetNbinsX()

    print("Size: " + str(Nbinsx))

    buf = np.frombuffer(hist.GetArray(), dtype='d', count=(Nbinsx+2))
    nphist = np.reshape(buf, Nbinsx+2)
    nphist = nphist[1:-1]

    xmin = hist.GetXaxis().GetXmin()
    xmax = hist.GetXaxis().GetXmax()
    dx = (xmax - xmin)/Nbinsx
    # Get bin centres from upper and lower edges
    nphistaxis = np.linspace(xmin+dx/2, xmax-dx/2, Nbinsx)

    return nphist, nphistaxis

def GetTH2D(hist):
    Nbinsx = hist.GetNbinsX()
    Nbinsy = hist.GetNbinsY()

    print("Size: " + str(Nbinsx) + ' x ' + str(Nbinsy))

    buf = np.frombuffer(hist.GetArray(), dtype='d', count=(Nbinsx+2)*(Nbinsy+2))
    nphist = np.reshape(buf, (Nbinsy+2, Nbinsx+2))
    nphist = nphist[1:-1, 1:-1]

    xmin = hist.GetXaxis().GetXmin()
    xmax = hist.GetXaxis().GetXmax()
    dx = (xmax - xmin)/Nbinsx
    # Get bin centres from upper and lower edges
    nphistxaxis = np.linspace(xmin+dx/2, xmax-dx/2, Nbinsx)

    ymin = hist.GetYaxis().GetXmin()
    ymax = hist.GetYaxis().GetXmax()
    dy = (ymax - ymin)/Nbinsy
    # Get bin centres from upper and lower edges
    nphistyaxis = np.linspace(ymin+dy/2, ymax-dy/2, Nbinsy)

    return nphist, nphistxaxis, nphistyaxis

def GetTree(T):
    nevents = T.GetEntries()
    leaves = T.GetListOfLeaves()
    # Dictionary holding ROOT leaves
    pyleaves = {}
    numleaves = leaves.GetEntries()
    # List of leaf names
    leavename = [None] * numleaves
    # Dictionary of leaf data
    data = {}

    print("Number of events: " + str(nevents) + " x " + str(numleaves) + " entries.")

    for i in range(numleaves):
        leavename[i] = leaves.At(i).GetName()
        pyleaves[leavename[i]] = leaves.At(i)
        data[leavename[i]] = np.zeros((nevents, 1))

    for i in range(nevents):
        # This sets a pointer or something...
        T.GetEntry(i)
        for l in leavename:
            # ... which GetValue() is looking at
            data[l][i] = pyleaves[l].GetValue()

    return data

def SaveMatfile(rootfile, matfile):
    f = TFile(rootfile)
    K = f.GetListOfKeys()

    dicttosave = {}

    for k in K:
        keyname = k.GetName()
        classname = k.GetClassName()

        print(keyname + " is of type " + classname)

        if classname == 'TH1D':
            hist, axis = GetTH1D(getattr(f, keyname))
            dicttosave[keyname] = hist
            dicttosave[keyname+"_xaxis"] = axis

        if classname == 'TH2D':
            hist, xaxis, yaxis = GetTH2D(getattr(f, keyname))
            dicttosave[keyname] = hist
            dicttosave[keyname+"_xaxis"] = xaxis
            dicttosave[keyname+"_yaxis"] = yaxis

        if classname == 'TTree':
            tree = GetTree(getattr(f, keyname))
            dicttosave[keyname] = tree


    scipy.io.savemat(matfile, mdict=dicttosave)



if len(sys.argv) > 2:
    rootfile = sys.argv[1]
    matfile = sys.argv[2]
elif len(sys.argv) == 2:
    rootfile = sys.argv[1]
    matfile = rootfile[:rootfile.find('root')-1] + '.mat'
elif len(sys.argv) == 1:
    rootfile = ['./data/'+f for f in os.listdir('./data/') if f.endswith('root')]
    existmatfile = ['./data/'+f for f in os.listdir('./data/') if f.endswith('mat')]
    matfile = [f[:f.find('root')-1] + '.mat' for f in rootfile if f[:f.find('root')-1] + '.mat' not in existmatfile]

if(len(rootfile[0]) == 1):
    SaveMatfile(rootfile, matfile)
else:
    for (r, m) in zip(rootfile, matfile):
        SaveMatfile(r, m)
