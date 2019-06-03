

{

// ichep dataset
double Ntrg_l1pl2f = 1.93963e+06; 
double Ntrg_l1pl2p = 225816; 
double Ntrg_l1fl2p = 35582; 
double Ntrg_tot = 2.4746e+06;

// fulldataset
Ntrg_l1pl2f = 5.74942e+06; 
Ntrg_l1pl2p = 552588; 
Ntrg_l1fl2p = 82113; 
Ntrg_tot = 7.21807e+06;

double Ntrg_not_l1pl2f = Ntrg_tot - Ntrg_l1pl2f;
double Ntrg_not_l1pl2p = Ntrg_tot - Ntrg_l1pl2p;
double Ntrg_not_l1fl2p = Ntrg_tot - Ntrg_l1fl2p;

double Ntrg_l1pl2f_err = sqrt(Ntrg_l1pl2f);
double Ntrg_l1pl2p_err = sqrt(Ntrg_l1pl2p);
double Ntrg_l1fl2p_err = sqrt(Ntrg_l1fl2p);

double Ntrg_not_l1pl2f_err = sqrt(Ntrg_not_l1pl2f);
double Ntrg_not_l1pl2p_err = sqrt(Ntrg_not_l1pl2p);
double Ntrg_not_l1fl2p_err = sqrt(Ntrg_not_l1fl2p);

double eff_trg_l1pl2f = Ntrg_l1pl2f/Ntrg_tot;
double eff_trg_l1pl2p = Ntrg_l1pl2p/Ntrg_tot;
double eff_trg_l1fl2p = Ntrg_l1fl2p/Ntrg_tot;

double eff_trg_l1pl2f_err = sqrt((pow(Ntrg_not_l1pl2f*Ntrg_l1pl2f_err,2)+pow(Ntrg_l1pl2f*Ntrg_not_l1pl2f_err,2))/pow(Ntrg_l1pl2f+Ntrg_not_l1pl2f,4));
double eff_trg_l1pl2p_err = sqrt((pow(Ntrg_not_l1pl2p*Ntrg_l1pl2p_err,2)+pow(Ntrg_l1pl2p*Ntrg_not_l1pl2p_err,2))/pow(Ntrg_l1pl2p+Ntrg_not_l1pl2p,4));
double eff_trg_l1fl2p_err = sqrt((pow(Ntrg_not_l1fl2p*Ntrg_l1fl2p_err,2)+pow(Ntrg_l1fl2p*Ntrg_not_l1fl2p_err,2))/pow(Ntrg_l1fl2p+Ntrg_not_l1fl2p,4));

std::cout << "eff_trg_l1pl2f = " << eff_trg_l1pl2f << " \\pm " << eff_trg_l1pl2f_err << std::endl;
std::cout << "eff_trg_l1pl2p = " << eff_trg_l1pl2p << " \\pm " << eff_trg_l1pl2p_err << std::endl;
std::cout << "eff_trg_l1fl2p = " << eff_trg_l1fl2p << " \\pm " << eff_trg_l1fl2p_err << std::endl;

}
