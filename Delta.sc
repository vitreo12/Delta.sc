Delta : UGen {
    *ar { arg in, thresh = 0.5;
        ^this.multiNew('audio', in, thresh)
    }
}