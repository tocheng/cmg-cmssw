

void printFilter(TH2D* h, Double_t cut){

  int nbinx = h->GetNbinsX();
  int nbiny = h->GetNbinsY();

  std::cout << "(";
  for (int i=1; i<nbinx+1; i++) {
    for (int j=1; j<nbiny+1; j++) {
      double cont = h->GetBinContent(i,j);
      if (cont>cut) {
        double xmin = h->GetXaxis()->GetBinLowEdge(i);
        double xmax = h->GetXaxis()->GetBinUpEdge(i);
        double ymin = h->GetYaxis()->GetBinLowEdge(j);
        double ymax = h->GetYaxis()->GetBinUpEdge(j);
        std::cout <<"!(eta>="<<xmin<<"&&eta<="<<xmax<<"&&phi>="<<ymin<<"&&phi<="<<ymax<<")&&";
      }
    }
  }
  std::cout << "(1))" << std::endl;

}
