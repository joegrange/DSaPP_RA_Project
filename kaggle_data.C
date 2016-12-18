#include "TF1.h"
#include "TF2.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TRandom3.h"
#include "TRandom.h"
#include <iostream>
#include "TBox.h"
#include <fstream>
#include "TH2F.h"
#include "TMarker.h"
#include "TPoint.h"
#include "TStyle.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGaxis.h"

using namespace std;
void combine_data(TString file1, TString file2);

void kaggle_data(){

  //gStyle->SetPadGridX(kTRUE);
  //gStyle->SetPadGridY(kTRUE);

  //for status reporting while it parses 3M entries
  int ctr = 0;
  int retval = 0;

 //combine_data("projects.csv","outcomes.csv");
 //return;

 char projectid[50];
 char teacher_acctid[50];
 char schoolid[50];
 char school_ncesid[50];
 double school_latitude;
 double school_longitude;
 char school_city[30];
 char school_state[10];
 unsigned int school_zip;
 char school_metro[20];
 char school_district[100];
 char school_county[100];
 char school_charter[5];
 char school_magnet[5];
 char school_year_round[5];
 char school_nlns[5];
 char school_kipp[5];
 char school_charter_ready_promise[5];
 char teacher_prefix[10];
 char teacher_teach_for_america[5];
 char teacher_ny_teaching_fellow[5];
 char primary_focus_subject[100];
 char primary_focus_area[100];
 char secondary_focus_subject[100];
 char secondary_focus_area[100];
 char resource_type[50];
 char poverty_level[100];
 char grade_level[50];
 float fulfillment_labor_materials, total_price_excluding_optional_support, total_price_including_optional_support;
 unsigned int students_reached;
 char eligible_double_your_impact_match[5];
 char eligible_almost_home_match[5];
 unsigned int yr, mo, day;
 
 TH1I *prefix = new TH1I("prefix","",3,0,3);
 TH1I *pov = new TH1I("pov","",4,0,4);
 TH1I *grades = new TH1I("grades","",4,0,4);

  FILE *pf = fopen("projects.csv","r");
  //FILE *pf = fopen("projects_test.csv","r");

 TH2I *pov_vs_reg[4];
 for (int i=0; i<4; i++) pov_vs_reg[i] = new TH2I(Form("pov_vs_reg_%d",i),"project poverty distribution (shape only); school latitude (deg); school longitude (deg)",40,-175,-65,40,10,70);

  TGraph *lat_pov = new TGraph();

  int pov_level = 999;

  while (!feof (pf)){
  
     ctr += 1;
     if (ctr%50000==0) cout<<" reading projects; entry "<<ctr<<endl;
     
     retval = fscanf(pf,"%50[^,],%50[^,],%50[^,],%50[^,],%lf,%lf,%30[^,],%10[^,],%d,%20[^,],%100[^,],%100[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%10[^,],%5[^,],%5[^,],%100[^,],%100[^,],%100[^,],%100[^,],%50[^,],%100[^,],%50[^,],%f,%f,%f,%d,%5[^,],%5[^,],%d-%d-%d",
     projectid,
	 teacher_acctid,
	 schoolid,
	 school_ncesid,
	 &school_latitude,
	 &school_longitude,
	 school_city,
	 school_state,
	 &school_zip,
	 school_metro,
	 school_district,
	 school_county,
	 school_charter,
	 school_magnet,
	 school_year_round,
	 school_nlns,
	 school_kipp,
	 school_charter_ready_promise,
	 teacher_prefix,
	 teacher_teach_for_america,
	 teacher_ny_teaching_fellow,
	 primary_focus_subject,
	 primary_focus_area,
	 secondary_focus_subject,
	 secondary_focus_area,
	 resource_type,
	 poverty_level,
	 grade_level,
	 &fulfillment_labor_materials,
	 &total_price_excluding_optional_support,
	 &total_price_including_optional_support,
	 &students_reached,
	 eligible_double_your_impact_match,
	 eligible_almost_home_match,
	 &yr,
	 &mo,
	 &day);
     
     //cout<<"lat "<<school_latitude<<" long "<<school_longitude<<endl;
     
     //cout<<" ret "<<retval<<endl;

     
	 if (retval!=37) continue;
	 //cout<<" grade "<<grade_level<<endl;
	 //cout<<" state "<<school_state<<endl;
     if (strcmp(teacher_prefix,"Mrs.")==0) prefix->Fill(0);
     if (strcmp(teacher_prefix,"Ms.")==0) prefix->Fill(1);
     if (strcmp(teacher_prefix,"Mr.")==0) prefix->Fill(2);
     
     if (strcmp(poverty_level,"low poverty")==0) pov_level = 1;
     if (strcmp(poverty_level,"moderate poverty")==0) pov_level = 2;
     if (strcmp(poverty_level,"high poverty")==0) pov_level = 3;
     if (strcmp(poverty_level,"highest poverty")==0) pov_level = 4;

     pov->Fill(pov_level);
     pov_vs_reg[pov_level-1]->Fill(school_longitude,school_latitude);

     if (strcmp(grade_level,"Grades PreK-2")==0) grades->Fill(0);
     if (strcmp(grade_level,"Grades 3-5")==0) grades->Fill(1);
     if (strcmp(grade_level,"Grades 6-8")==0) grades->Fill(2);
     if (strcmp(grade_level,"Grades 9-12")==0) grades->Fill(3);

     lat_pov->SetPoint(lat_pov->GetN(),school_latitude,pov_level);

     //cout<<" poverty_level "<<poverty_level<<endl;

//      cout<<
//      " \nprojectid\t"<<projectid<<
//      " \nteacher_acctid\t"<<teacher_acctid<<
//      " \nschoolid\t"<<schoolid<<
//      " \nschool_ncesid\t"<<school_ncesid<<
//      " \nschool_latitude\t"<<school_latitude<<
//      " \nschool_longitude\t"<<school_longitude<<
//      " \nschool_city\t"<<school_city<<
//      " \nschool_state\t"<<school_city<<
//      " \nschool_zip\t"<<school_zip<<
//      " \nschool_metro\t"<<school_metro<<
//      " \nschool_district\t"<<school_district<<
//      " \nschool_county\t"<<school_county<<
//      " \nschool_charter\t"<<school_charter<<
//      " \nschool_magnet\t"<<school_magnet<<
//      " \nschool_year_round\t"<<school_year_round<<
//      " \nschool_nlns\t"<<school_nlns<<
//      " \nschool_kipp\t"<<school_kipp<<
//      " \nteacher_prefix\t"<<teacher_prefix<<
//      " \nteacher_teach_for_america\t"<<teacher_teach_for_america<<
//      " \nteacher_ny_teaching_fellow\t"<<teacher_ny_teaching_fellow<<
//      " \npoverty_level\t"<<poverty_level<<
//      " \ngrade_level\t"<<grade_level<<
//      " \nfulfillment_labor_materials\t"<<fulfillment_labor_materials<<
//      " \ntotal_price_excluding_optional_support\t"<<total_price_excluding_optional_support<<
//      " \ntotal_price_including_optional_support\t"<<total_price_including_optional_support<<
//      " \nstudents_reached\t"<<students_reached<<
//      " \nyr-mo-day "<<yr<<"-"<<mo<<"-"<<day<<
//      endl;
     
  }
    
  cout<<" prefix fracs: Mrs. "<<prefix->GetBinContent(1) / prefix->Integral()<<" Ms. "<<prefix->GetBinContent(2) / prefix->Integral()<<" Mr. "<<prefix->GetBinContent(3) / prefix->Integral()<<" total "<<prefix->Integral()<<" entries compared to "<<ctr<<" total "<<endl;
  cout<<" female "<<(prefix->GetBinContent(1) + prefix->GetBinContent(2))/prefix->Integral()<<" male "<<prefix->GetBinContent(3) / prefix->Integral()<<endl;
  
  TCanvas *myc = new TCanvas();
  prefix->Draw();
  myc->Print("teach_prefix.eps");
  
  cout<<" poverty fracs: low "<<pov->GetBinContent(1) / pov->Integral()<<" moderate "<<pov->GetBinContent(2) / pov->Integral()<<" high "<<pov->GetBinContent(3) / pov->Integral()<<" highest "<<pov->GetBinContent(4) / pov->Integral()<<" total entries "<<pov->Integral()<<" compared to "<<ctr<<" total "<<endl;

  TCanvas *myc2 = new TCanvas();
  pov->Draw();
  myc2->Print("poverty.eps");

  cout<<" grade fracs: prek-2 "<<grades->GetBinContent(1) / grades->Integral()<<" 3-5 "<<grades->GetBinContent(2) / grades->Integral()<<" 6-8 "<<grades->GetBinContent(3) / grades->Integral()<<" 9-12 "<<grades->GetBinContent(4) / grades->Integral()<<" total entries "<<grades->Integral()<<" compared to "<<ctr<<" total "<<endl;

  grades->SetMinimum(0);

  TCanvas *myc3 = new TCanvas();
  grades->Draw();
  myc3->Print("grades.eps");
 
  lat_pov->SetMarkerStyle(20);
 
  for (int i=0; i<4; i++) pov_vs_reg[i]->SetLineWidth(2);
  pov_vs_reg[0]->SetLineColor(kGreen);
  pov_vs_reg[1]->SetLineColor(kBlack);
  pov_vs_reg[2]->SetLineColor(kBlue);
  pov_vs_reg[3]->SetLineColor(kRed);
 
  gStyle->SetOptStat(0000);
 
  TLegend *lp = new TLegend(0.759, 0.698, 0.875, 0.863);
  lp->AddEntry(pov_vs_reg[0],"low", "l");
  lp->AddEntry(pov_vs_reg[1],"moderate", "l");
  lp->AddEntry(pov_vs_reg[2],"high", "l");
  lp->AddEntry(pov_vs_reg[3],"highest", "l");
  lp->SetFillColor(0);
  lp->SetLineColor(1);
  lp->SetTextSize(0.03);

  TCanvas *cci = new TCanvas();
  pov_vs_reg[0]->Draw("box");
  pov_vs_reg[1]->Draw("box same");
  pov_vs_reg[2]->Draw("box same");
  pov_vs_reg[3]->Draw("box same");
  lp->Draw();
  cci->Print("poverty_by_region.eps");

  TCanvas *cci2 = new TCanvas();
  pov_vs_reg[0]->Draw("box");
  lp->Draw();
  cci2->Print("low_poverty.eps");
 
  // TCanvas *corrP = new TCanvas();
//   lat_pov->Draw("ap");
//   corrP->Print("corrP.png");
//   
 ctr = 0;
 
 char is_exciting[5];
 char at_least_1_teacher_referred_donor[5];
 char fully_funded[5];
 char at_least_1_green_donation[5];
 char great_chat[5];
 char three_or_more_non_teacher_referred_donors[5];
 char one_non_teacher_referred_donor_giving_100_plus[5];
 char donation_from_thoughtful_donor[5];
 float great_messages_proportion,teacher_referred_count,non_teacher_referred_count;
 
 int cnt_exc = 0;
 int cnt_tref = 0;
 int cnt_ful = 0;
 int cnt_grn = 0;
 int cnt_grc = 0;
 int cnt_ref = 0;
 int cnt_100p = 0;
 int cnt_tht = 0;
 
 FILE *oc = fopen("outcomes.csv","r");

  while (!feof (oc)){
  
     ctr += 1;
     if (ctr%50000==0) cout<<" reading outcomes; entry "<<ctr<<endl;
     
     retval = fscanf(oc,"%50[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%f,%f,%f",
     projectid,
     is_exciting,
     at_least_1_teacher_referred_donor,
     fully_funded,
     at_least_1_green_donation,
     great_chat,
     three_or_more_non_teacher_referred_donors,
     one_non_teacher_referred_donor_giving_100_plus,
     donation_from_thoughtful_donor,
     &great_messages_proportion,
     &teacher_referred_count,
     &non_teacher_referred_count);
     
     //cout<<" ret "<<retval<<endl;

     if (retval!=12) continue;
     
     if (strcmp(is_exciting,"t")==0) cnt_exc += 1;
     if (strcmp(at_least_1_teacher_referred_donor,"t")==0) cnt_tref += 1;
     if (strcmp(fully_funded,"t")==0) cnt_ful += 1;
     if (strcmp(at_least_1_green_donation,"t")==0) cnt_grn += 1;
     if (strcmp(great_chat,"t")==0) cnt_grc += 1;
     if (strcmp(three_or_more_non_teacher_referred_donors,"t")==0) cnt_ref += 1;
     if (strcmp(one_non_teacher_referred_donor_giving_100_plus,"t")==0) cnt_100p += 1;
     if (strcmp(donation_from_thoughtful_donor,"t")==0) cnt_100p += 1;
     

//      cout<<" projectid "<<projectid<<" \n is_exciting "<<is_exciting<<" \n at_least_1_teacher_referred_donor "<<at_least_1_teacher_referred_donor<<
//      " \n fully_funded "<<fully_funded<<" \n at_least_1_green_donation "<<at_least_1_green_donation<<" \n great_chat "<<great_chat<<" \n three_or_more_non_teacher_referred_donors "
//      <<three_or_more_non_teacher_referred_donors<<" \n one_non_teacher_referred_donor_giving_100_plus "<<one_non_teacher_referred_donor_giving_100_plus
//      <<" \n donation_from_thoughtful_donor "<<donation_from_thoughtful_donor<<" \n great_messages_proportion "<<great_messages_proportion<<
//      " \n teacher_referred_count "<<teacher_referred_count<<" \n non_teacher_referred_count "<<non_teacher_referred_count<<endl; 
  }
  
  cout<<" fractions: \n exciting\t"<<(float)cnt_exc/ctr<<
  "\n at_least_1_teacher_referred_donor\t"<<(float)cnt_tref/ctr<<
  "\n fully_funded\t"<<(float)cnt_ful/ctr<<
  "\n at_least_1_green_donation\t"<<(float)cnt_grn/ctr<<
  "\n great_chat\t"<<(float)cnt_grc/ctr<<
  "\n three_or_more_non_teacher_referred_donors\t"<<(float)cnt_ref/ctr<<
  "\n at_least_1_teacher_referred_donor\t"<<(float)cnt_100p/ctr<<
  "\n donation_from_thoughtful_donor\t"<<(float)cnt_tht/ctr<<endl;
  
 FILE *fp = fopen("donations.csv","r");

 char donationid[40];
//  char projectid[40];
 char donor_acctid[40];
 char donor_city[30];
 char donor_state[30];
 int donor_zip;
 char is_teacher_acct[5];
 //unsigned int yr, mo, day, 
 unsigned int hr, min, sec, ms;
 float donation_to_project,donation_optional_support,donation_total;
 char dollar_amount[20];
 char donation_included_optional_support[5];
 char payment_method[20];
 char payment_included_acct_credit[5];
 char payment_included_campaign_gift_card[5];
 char payment_included_web_purchased_gift_card[5];
 char payment_was_promo_matched[5];
 char via_giving_page[5];
 char for_honoree[5];
 char donation_message[1000];

 float min_rng = 0.;
 float max_rng = 400.;

  TH1F *donation_proj = new TH1F("donation_proj","donation amount; donation ($); frequency",100,min_rng,max_rng);
  TH1F *donation_supp = new TH1F("donation_supp","donation amount; donation ($); frequency",100,min_rng,max_rng);
  TH1F *donation_total_h = new TH1F("donation_total_h","donation amount; donation ($); frequency",100,min_rng,max_rng);
  TH1F *donation_proj_frac = new TH1F("donation_proj_frac","donation by type; fraction of total; frequency",100,0,1.01);
  TH1F *donation_supp_frac = new TH1F("donation_supp_frac","donation by type; fraction of total; frequency",100,0,1.01);

  TH1F *hr_of_day = new TH1F("hr_of_day","donation hour (time zone?); hour; frequency",24,0,24);
  TH1F *yr_h = new TH1F("yr_h","donation year; year; frequency",20,2000,2020);

  TH1F *teacher_acct = new TH1F("teacher_acct","teacher account?; boolean (0==no, 1==yes); frequency",2,0,2);
  TH1F *opt_support = new TH1F("opt_support","Optional support?; boolean (0==no, 1==yes); frequency",2,0,2);
  TH1F *inc_acct_cred = new TH1F("inc_acct_cred","Donation included acct credit?; boolean (0==no, 1==yes); frequency",2,0,2);
  TH1F *inc_camp_gc = new TH1F("inc_camp_gc","Donation included campaign gift card?; boolean (0==no, 1==yes); frequency",2,0,2);
  TH1F *inc_web_gc = new TH1F("inc_camp_web_gc","Donation included web-purchased gift card?; boolean (0==no, 1==yes); frequency",2,0,2);
  TH1F *promo_match = new TH1F("promo_match","Promo matched?; boolean (0==no, 1==yes); frequency",2,0,2);
  TH1F *giving_page = new TH1F("giving_page","Donation through giving page?; boolean (0==no, 1==yes); frequency",2,0,2);
  TH1F *for_honoreeh = new TH1F("for_honoreeh","Donation for honoree?; boolean (0==no, 1==yes); frequency",2,0,2);

  //reset
  ctr = 0;

  //note: !feof includes a bogus final entry since it .  Essentially double-counts the last entry in the file.  As long as stats are high (they are in this case) this won't change conclusions but should be cleaned up.
  while (!feof (fp)){
  
     ctr += 1;
     if (ctr%50000==0) cout<<" reading donations; entry "<<ctr<<endl;
     
     retval = fscanf(fp,"%50[^,],%50[^,],%50[^,],%50[^,],%10[^,],%d,%5[^,],%d-%d-%d %d:%d:%d.%d,%f,%f,%f,%20[^,],%5[^,],%20[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%1000[^\n]",
     donationid,projectid,donor_acctid,donor_city,donor_state,&donor_zip,is_teacher_acct,&yr,&mo,&day,&hr,&min,&sec,&ms,
     &donation_to_project,&donation_optional_support,&donation_total,dollar_amount,
     donation_included_optional_support,payment_method,
     payment_included_acct_credit,payment_included_campaign_gift_card,payment_included_web_purchased_gift_card,payment_was_promo_matched,via_giving_page,for_honoree,
     donation_message);
     
     donation_proj->Fill(donation_to_project);
     donation_supp->Fill(donation_optional_support);
     donation_total_h->Fill(donation_total);
     
     donation_proj_frac->Fill(donation_to_project / donation_total);
     donation_supp_frac->Fill(donation_optional_support / donation_total);
     
     hr_of_day->Fill(hr);
     yr_h->Fill(yr);
     
//      if (donation_to_project + donation_optional_support != donation_total){
//      	cout<<" "<<donation_to_project<<" + "<<donation_optional_support<<" != "<<donation_total<<endl;
//         cout<<" sum "<<donation_to_project + donation_optional_support<<endl;
//      }
     
     //cout<<" proj frac "<<donation_to_project / donation_total<<endl;
     //cout<<" supp frac "<<donation_optional_support / donation_total<<endl;
     //cout<<" "<<is_teacher_acct<<" results for f "<<strcmp(is_teacher_acct,"f")<<endl;
     if (strcmp(is_teacher_acct,"t")==0) teacher_acct->Fill(1);
     if (strcmp(is_teacher_acct,"f")==0) teacher_acct->Fill(0);

     if (strcmp(donation_included_optional_support,"t")==0) opt_support->Fill(1);
     if (strcmp(donation_included_optional_support,"f")==0) opt_support->Fill(0);
     
     if (strcmp(payment_included_acct_credit,"t")==0) inc_acct_cred->Fill(1);
     if (strcmp(payment_included_acct_credit,"f")==0) inc_acct_cred->Fill(0);

     if (strcmp(payment_included_campaign_gift_card,"t")==0) inc_camp_gc->Fill(1);
     if (strcmp(payment_included_campaign_gift_card,"f")==0) inc_camp_gc->Fill(0);

     if (strcmp(payment_included_web_purchased_gift_card,"t")==0) inc_web_gc->Fill(1);
     if (strcmp(payment_included_web_purchased_gift_card,"f")==0) inc_web_gc->Fill(0);

     if (strcmp(payment_was_promo_matched,"t")==0) promo_match->Fill(1);
     if (strcmp(payment_was_promo_matched,"f")==0) promo_match->Fill(0);

     if (strcmp(via_giving_page,"t")==0) giving_page->Fill(1);
     if (strcmp(via_giving_page,"f")==0) giving_page->Fill(0);

     if (strcmp(for_honoree,"t")==0) for_honoreeh->Fill(1);
     if (strcmp(for_honoree,"f")==0) for_honoreeh->Fill(0);

     
     //retval = fscanf(fp,"%50[^,],%50[^,],%50[^,]",donationid,projectid,donor_acctid);
     //cout<<" donationid "<<donationid<<" projectid "<<projectid<<" donor_acctid "<<donor_acctid<<endl;//<<" projectid "<<projectid<<" donor_acctid "<<donor_acctid<<" donor_city "<<donor_city<<" donor_state "<<donor_state<<endl; 
     //cout<<" donationid "<<donationid<<" projectid "<<projectid<<" donor_acctid "<<donor_acctid<<" donor_city "<<donor_city<<" donor_state "<<donor_state<<" donor_zip "<<donor_zip<<endl; 
     //cout<<" ret val "<<retval<<endl;
     //cout<<" yr "<<yr<<" mo "<<mo<<" day "<<day<<" hr "<<hr<<" min "<<min<<" sec "<<sec<<" ms "<<ms<<endl;
     //cout<<yr<<"-"<<mo<<"-"<<day<<" "<<hr<<":"<<min<<":"<<sec<<"."<<ms<<endl;
     //cout<<" donation_to_project "<<donation_to_project<<" donation_optional_support "<<donation_optional_support<<" donation_total "<<donation_total<<" dollar_amount "<<dollar_amount<<endl;
     //cout<<" donation_included_optional_support "<<donation_included_optional_support<<" payment_method "<<payment_method<<" payment_included_acct_credit "<<payment_included_acct_credit<<
     //" payment_included_campaign_gift_card "<<payment_included_campaign_gift_card<<" payment_included_web_purchased_gift_card "<<payment_included_web_purchased_gift_card<<" payment_was_promo_matched "<<payment_was_promo_matched<<" via_giving_page "<<via_giving_page<<" for_honoree "<<for_honoree<<endl;
     //cout<<" donation_included_optional_support "<<donation_included_optional_support<<"  "<<payment_method<<"  "<<payment_included_acct_credit<<
     //"  "<<payment_included_campaign_gift_card<<"  "<<payment_included_web_purchased_gift_card<<"  "<<payment_was_promo_matched<<"  "<<via_giving_page<<"  "<<for_honoree<<endl;
     //cout<<" donation_to_project "<<donation_to_project<<" donation amt "<<donation_total<<" donation_message "<<donation_message<<endl;
     //if (yr>2014) cout<<" year "<<yr<<"!"<<endl;
  }

  donation_total_h->SetLineWidth(2);
  donation_total_h->SetLineColor(kBlack);
  donation_total_h->SetMaximum(1.3*donation_total_h->GetMaximum());
  donation_proj->SetLineWidth(2);
  donation_proj->SetLineColor(kRed);
  donation_supp->SetLineWidth(2);
  donation_supp->SetLineColor(kBlue);
  donation_proj_frac->SetLineWidth(2);
  donation_proj_frac->SetLineColor(kRed);
  donation_supp_frac->SetLineWidth(2);
  donation_supp_frac->SetLineColor(kBlue);
  
  gStyle->SetOptStat(1100);
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);

  TLegend *l1 = new TLegend(0.4, 0.75, 0.7, 0.85);
  l1->AddEntry(donation_proj_frac,"to project", "l");
  l1->AddEntry(donation_supp_frac,"to optional support", "l");
  l1->AddEntry(donation_total_h,"total", "l");
  l1->SetFillColor(0);
  l1->SetLineColor(1);
  l1->SetTextSize(0.03);

  TLegend *l2 = new TLegend(0.4, 0.75, 0.7, 0.85);
  l2->AddEntry(donation_proj_frac,"to project", "l");
  l2->AddEntry(donation_supp_frac,"to optional support", "l");
  l2->SetFillColor(0);
  l2->SetLineColor(1);
  l2->SetTextSize(0.03);

  TCanvas *c2 = new TCanvas();
  hr_of_day->Draw();
  c2->Print("donation_hr.eps");

  yr_h->GetYaxis()->SetTitleOffset(1.2);

  TCanvas *c3 = new TCanvas();
  yr_h->Draw();
  c3->Print("donation_yr.eps");

  gStyle->SetOptStat(0000);

  donation_proj_frac->SetMaximum(1.3*donation_proj_frac->GetMaximum());
  
  TCanvas *c5 = new TCanvas();
  donation_proj_frac->Draw();
  donation_supp_frac->Draw("sames");
  l2->Draw();
  c5->Print("donations_frac.eps");

  TCanvas *c1 = new TCanvas();
  donation_total_h->Draw();
  donation_proj->Draw("same");
  donation_supp->Draw("sames");
  l1->Draw();
  c1->Print("donations.eps");

  teacher_acct->SetMinimum(0);
  teacher_acct->SetLineWidth(2);
  teacher_acct->GetYaxis()->SetTitleOffset(1.3);

  TCanvas *c4 = new TCanvas();
  teacher_acct->Draw();
  c4->Print("teacher_acct.eps");

  opt_support->SetMinimum(0);
  opt_support->SetLineWidth(2);
  opt_support->GetYaxis()->SetTitleOffset(1.3);

  TCanvas *c60 = new TCanvas();
  opt_support->Draw();
  c60->Print("opt_support.eps");

  inc_acct_cred->SetMinimum(0);
  inc_acct_cred->SetLineWidth(2);
  inc_acct_cred->GetYaxis()->SetTitleOffset(1.3);

  TCanvas *c6 = new TCanvas();
  inc_acct_cred->Draw();
  c6->Print("acct_credit.eps");

  inc_camp_gc->SetMinimum(0);
  inc_camp_gc->SetLineWidth(2);
  inc_camp_gc->GetYaxis()->SetTitleOffset(1.3);

  TCanvas *c7 = new TCanvas();
  inc_camp_gc->Draw();
  c7->Print("inc_camp_gc.eps");

  promo_match->SetMinimum(0);
  promo_match->SetLineWidth(2);
  promo_match->GetYaxis()->SetTitleOffset(1.3);

  TCanvas *c8 = new TCanvas();
  promo_match->Draw();
  c8->Print("promo_match.eps");

  giving_page->SetMinimum(0);
  giving_page->SetLineWidth(2);
  giving_page->GetYaxis()->SetTitleOffset(1.3);

  TCanvas *c9 = new TCanvas();
  giving_page->Draw();
  c9->Print("giving_page.eps");

  for_honoreeh->SetMinimum(0);
  for_honoreeh->SetLineWidth(2);
  for_honoreeh->GetYaxis()->SetTitleOffset(1.3);

  TCanvas *c10 = new TCanvas();
  for_honoreeh->Draw();
  c10->Print("for_honoreeh.eps");

  FILE *combd = fopen("combined_data_f.csv","r");

  ctr = 0;

  int total = 0;
  int total_male = 0;
  int total_female = 0;
  int ff_male = 0;
  int ff_female = 0;
  int ie_male = 0;
  int ie_female = 0;
  
  TH1I *nstudents_ff = new TH1I("nstudents_ff",";number of students affected by project;frequency",200,0,200);
  TH1I *nstudents_nff = new TH1I("nstudents_nff",";number of students reached by project;frequency",200,0,200);

  TH1F *cost_ff = new TH1F("cost_ff",";total cost of project;frequency",100,0,3000);
  TH1F *cost_nff = new TH1F("cost_nff",";total cost of project;frequency",100,0,3000);

  TH2F *cost_nstudents_all = new TH2F("cost_nstudents_all",";total cost of project ($);students reached",30,0,1500,10,0,500);
  TH2F *cost_nstudents_ff = new TH2F("cost_nstudents_ff",";total cost of project ($);students reached",30,0,1500,10,0,500);
  TH2F *cost_nstudents_nff = new TH2F("cost_nstudents_nff",";total cost of project ($);students reached",30,0,1500,10,0,500);

  TH1I *pov_all = new TH1I("pov_all",";povery level (0: low, 4:highest);frequency",4,0,4);
  TH1I *pov_ff = new TH1I("pov_ff",";povery level (0: low, 4:highest);frequency",4,0,4);
  TH1I *pov_nff = new TH1I("pov_nff",";povery level (0: low, 4:highest);frequency",4,0,4);

  //note: !feof includes a bogus final entry since it essentially double-counts the last entry in the file.  As long as stats are high (they are in this case) this won't change conclusions but should be cleaned up.
  while (!feof (combd)){

     ctr += 1;
     retval = fscanf(combd,
     //"%50[^,],%50[^,],%50[^,],%50[^,],%lf,%lf,%30[^,],%10[^,],%d,%20[^,],%100[^,],%100[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%10[^,],%5[^,],%5[^,],%100[^,],%100[^,],%100[^,],%100[^,],%50[^,],%100[^,],%50[^,],%f,%f,%f,%d,%5[^,],%5[^,],%d,%d,%d,%5[^,],%5[^,],%5[^,]",
     "%50[^,],%50[^,],%50[^,],%50[^,],%lf,%lf,%30[^,],%10[^,],%d,%20[^,],%100[^,],%100[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%10[^,],%5[^,],%5[^,],%100[^,],%100[^,],%100[^,],%100[^,],%50[^,],%100[^,],%50[^,],%f,%f,%f,%d,%5[^,],%5[^,],%d,%d,%d,%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%f,%f,%f",
     projectid,
	 teacher_acctid,
	 schoolid,
	 school_ncesid,
	 &school_latitude,
	 &school_longitude,
	 school_city,
	 school_state,
	 &school_zip,
	 school_metro,
	 
	 school_district,
	 school_county,
	 school_charter,
	 school_magnet,
	 school_year_round,
	 school_nlns,
	 school_kipp,
	 school_charter_ready_promise,
	 teacher_prefix,
	 teacher_teach_for_america,
	 
	 teacher_ny_teaching_fellow,
	 primary_focus_subject,
	 primary_focus_area,
	 secondary_focus_subject,
	 secondary_focus_area,
	 resource_type,
	 poverty_level,
	 grade_level,
	 &fulfillment_labor_materials,
	 &total_price_excluding_optional_support,
	 
	 &total_price_including_optional_support,
	 &students_reached,
	 eligible_double_your_impact_match,
	 eligible_almost_home_match,
	 &yr,
	 &mo,
	 &day,
	 is_exciting,
     at_least_1_teacher_referred_donor,
     fully_funded,
 
     at_least_1_green_donation,
     great_chat,
     three_or_more_non_teacher_referred_donors,
     one_non_teacher_referred_donor_giving_100_plus,
     donation_from_thoughtful_donor,
     &great_messages_proportion,
     &teacher_referred_count,
     &non_teacher_referred_count);

     //cout<<"retval "<<retval<<" projectid "<<projectid<<" teacher_acctid "<<teacher_acctid<<" schoolid "<<schoolid<<" school_latitude "<<school_latitude<<" school_metro "<<school_metro<<" teacher_prefix "<<teacher_prefix<<" total_price_excluding_optional_support "<<total_price_excluding_optional_support<<" fully_funded "<<fully_funded<<" non_teacher_referred_count "<<non_teacher_referred_count<<" ctr "<<ctr<<endl;
     if (retval!=48) continue;
  
     total += 1;
     if (strcmp(teacher_prefix,"Mr.")==0) total_male += 1;
     if (strcmp(teacher_prefix,"Mrs.")==0||strcmp(teacher_prefix,"Ms.")==0) total_female += 1;
     if (strcmp(teacher_prefix,"Mr.")==0&&strcmp(fully_funded,"t")==0) ff_male += 1;
     if ((strcmp(teacher_prefix,"Mrs.")==0||strcmp(teacher_prefix,"Ms.")==0)&&strcmp(fully_funded,"t")==0) ff_female += 1;

     if (strcmp(teacher_prefix,"Mr.")==0&&strcmp(is_exciting,"t")==0) ie_male += 1;
     if ((strcmp(teacher_prefix,"Mrs.")==0||strcmp(teacher_prefix,"Ms.")==0)&&strcmp(is_exciting,"t")==0) ie_female += 1;
  
     if (strcmp(fully_funded,"t")==0) nstudents_ff->Fill(students_reached);
     if (strcmp(fully_funded,"f")==0) nstudents_nff->Fill(students_reached);

     if (strcmp(fully_funded,"t")==0) cost_ff->Fill(total_price_including_optional_support);
     if (strcmp(fully_funded,"f")==0) cost_nff->Fill(total_price_including_optional_support);
  
     if (strcmp(poverty_level,"low poverty")==0) pov_level = 1;
     if (strcmp(poverty_level,"moderate poverty")==0) pov_level = 2;
     if (strcmp(poverty_level,"high poverty")==0) pov_level = 3;
     if (strcmp(poverty_level,"highest poverty")==0) pov_level = 4;

     pov_all->Fill(pov_level);
     cost_nstudents_all->Fill(total_price_including_optional_support,students_reached);
     if (strcmp(fully_funded,"t")==0){
       cost_nstudents_ff->Fill(total_price_including_optional_support,students_reached);
       pov_ff->Fill(pov_level);
     }
     if (strcmp(fully_funded,"f")==0){
       cost_nstudents_nff->Fill(total_price_including_optional_support,students_reached);
       pov_nff->Fill(pov_level);
     }
     
     if (strcmp(teacher_prefix,"Mr.")==0&&strcmp(is_exciting,"t")==0&&pov_level==4&&students_reached>40) cout<<" int projectid "<<projectid<<" students reached "<<students_reached<<endl; 

  }

  cout<<" ff female "<<(float)ff_female / total<<" ff male "<<(float)ff_male/total<<endl;
  cout<<" ie female "<<(float)ie_female / total<<" ie male "<<(float)ie_male/total<<endl;
  cout<<" total female "<<total_female<<" total male "<<total_male<<endl;

  for (int i=1; i<=cost_nstudents_ff->GetNbinsX(); i++){
    for (int j=1; j<=cost_nstudents_ff->GetNbinsY(); j++){
    
    if (cost_nstudents_all->GetBinContent(i,j)>0) cost_nstudents_ff->SetBinContent(i,j,cost_nstudents_ff->GetBinContent(i,j) / cost_nstudents_all->GetBinContent(i,j));
    
    }
  }

  nstudents_ff->SetLineWidth(2);
  nstudents_ff->SetLineColor(kBlue);
  nstudents_nff->SetLineWidth(2);
  nstudents_nff->SetLineColor(kRed);

  cost_ff->SetLineWidth(2);
  cost_ff->SetLineColor(kBlue);
  cost_nff->SetLineWidth(2);
  cost_nff->SetLineColor(kRed);

  pov_all->SetLineWidth(2);
  pov_all->SetLineColor(kBlack);
  pov_ff->SetLineWidth(2);
  pov_ff->SetLineColor(kBlue);
  pov_nff->SetLineWidth(2);
  pov_nff->SetLineColor(kRed);

  TLegend *lp2 = new TLegend(0.687, 0.700, 0.872, 0.865);
  lp2->AddEntry(nstudents_ff,"fully funded", "l");
  lp2->AddEntry(nstudents_nff,"not fully funded", "l");
  lp2->SetFillColor(0);
  lp2->SetLineColor(1);
  lp2->SetTextSize(0.03);

  TLegend *lp3 = new TLegend(0.231, 0.700, 0.415, 0.865);
  lp3->AddEntry(nstudents_ff,"fully funded", "l");
  lp3->AddEntry(nstudents_nff,"not fully funded", "l");
  lp3->AddEntry(pov_all,"total proposed", "l");
  lp3->SetFillColor(0);
  lp3->SetLineColor(1);
  lp3->SetTextSize(0.03);

  TCanvas *studC = new TCanvas();
  nstudents_ff->Draw("");
  nstudents_nff->Draw("same");
  lp2->Draw();
  studC->Print("students_ff.eps");

  TCanvas *studC2 = new TCanvas();
  cost_ff->Draw("");
  cost_nff->Draw("same");
  lp2->Draw();
  studC2->Print("cost_ff.eps");
  
  TCanvas *dsaf = new TCanvas();
  cost_nstudents_ff->Draw("colz");
  dsaf->Print("ff_frac.eps");
  
  cost_nstudents_ff->SetLineWidth(2);
  cost_nstudents_ff->SetLineColor(kBlue);
  cost_nstudents_nff->SetLineWidth(2);
  cost_nstudents_nff->SetLineColor(kRed);
  
  TCanvas *cc2 = new TCanvas();
  cost_nstudents_ff->Draw("box");
  cost_nstudents_nff->Draw("box same");
  lp2->Draw();
  cc2->Print("cost_stud_ff.eps");

  pov_ff->SetMaximum(1.7*pov_ff->GetMaximum());

  TCanvas *povC = new TCanvas();
  pov_ff->Draw("");
  pov_nff->Draw("same");
  pov_all->Draw("same");
  lp3->Draw();
  povC->Print("pov_ff.eps");

}

void combine_data(TString name1, TString name2){

 //projects vars
 char projectid_1[50];
 char projectid_2[50];
 char projectid_3[50];
 char teacher_acctid[50];
 char schoolid[50];
 char school_ncesid[50];
 double school_latitude;
 double school_longitude;
 char school_city[30];
 char school_state[10];
 unsigned int school_zip;
 char school_metro[20];
 char school_district[100];
 char school_county[100];
 char school_charter[5];
 char school_magnet[5];
 char school_year_round[5];
 char school_nlns[5];
 char school_kipp[5];
 char school_charter_ready_promise[5];
 char teacher_prefix[10];
 char teacher_teach_for_america[5];
 char teacher_ny_teaching_fellow[5];
 char primary_focus_subject[100];
 char primary_focus_area[100];
 char secondary_focus_subject[100];
 char secondary_focus_area[100];
 char resource_type[50];
 char poverty_level[100];
 char grade_level[50];
 float fulfillment_labor_materials, total_price_excluding_optional_support, total_price_including_optional_support;
 unsigned int students_reached;
 char eligible_double_your_impact_match[5];
 char eligible_almost_home_match[5];
 unsigned int yr, mo, day;
 
 //outcomes vars
 char is_exciting[5];
 char at_least_1_teacher_referred_donor[5];
 char fully_funded[5];
 char at_least_1_green_donation[5];
 char great_chat[5];
 char three_or_more_non_teacher_referred_donors[5];
 char one_non_teacher_referred_donor_giving_100_plus[5];
 char donation_from_thoughtful_donor[5];
 float great_messages_proportion,teacher_referred_count,non_teacher_referred_count;

  int retval = 0;
  int ctr1 = 0;
  int ctr2 = 0;

  ofstream combined_file;

  combined_file.open("combined_data.csv");
  //get nlines 
  int n_lines_1 = 0;
  ifstream f(name1.Data());
  string line;
  for (int i = 0; std::getline(f, line); ++i)
    n_lines_1++;
    
  int n_lines_2 = 0;
  ifstream b(name2.Data());
  string line2;
  for (int i = 0; std::getline(b, line2); ++i)
    n_lines_2++;
  FILE *pf = fopen("projects.csv","r");

  FILE *f1 = fopen(name1.Data(),"r");
  FILE *f2 = fopen(name2.Data(),"r"); 
 
  bool matched = 0;
 
    while (!feof (f2)){
  
     ctr1 += 1;
     cout<<" reading outcomes; entry "<<ctr1<<endl;

      retval = fscanf(f2,"%50[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%f,%f,%f",
      projectid_2,
      is_exciting,
      at_least_1_teacher_referred_donor,
      fully_funded,
      at_least_1_green_donation,
      great_chat,
      three_or_more_non_teacher_referred_donors,
      one_non_teacher_referred_donor_giving_100_plus,
      donation_from_thoughtful_donor,
      &great_messages_proportion,
      &teacher_referred_count,
      &non_teacher_referred_count);

      //cout<<" retval "<<retval<<endl;

      if (retval!=12) continue;


//      cout<<
//      " \nprojectid\t"<<projectid_1<<
//      " \nteacher_acctid\t"<<teacher_acctid<<
//      " \nschoolid\t"<<schoolid<<
//      " \nschool_ncesid\t"<<school_ncesid<<
//      " \nschool_latitude\t"<<school_latitude<<
//      " \nschool_longitude\t"<<school_longitude<<
//      " \nschool_city\t"<<school_city<<
//      " \nschool_state\t"<<school_city<<
//      " \nschool_zip\t"<<school_zip<<
//      " \nschool_metro\t"<<school_metro<<
//      " \nschool_district\t"<<school_district<<
//      " \nschool_county\t"<<school_county<<
//      " \nschool_charter\t"<<school_charter<<
//      " \nschool_magnet\t"<<school_magnet<<
//      " \nschool_year_round\t"<<school_year_round<<
//      " \nschool_nlns\t"<<school_nlns<<
//      " \nschool_kipp\t"<<school_kipp<<
//      " \nteacher_prefix\t"<<teacher_prefix<<
//      " \nteacher_teach_for_america\t"<<teacher_teach_for_america<<
//      " \nteacher_ny_teaching_fellow\t"<<teacher_ny_teaching_fellow<<
//      " \npoverty_level\t"<<poverty_level<<
//      " \ngrade_level\t"<<grade_level<<
//      " \nfulfillment_labor_materials\t"<<fulfillment_labor_materials<<
//      " \ntotal_price_excluding_optional_support\t"<<total_price_excluding_optional_support<<
//      " \ntotal_price_including_optional_support\t"<<total_price_including_optional_support<<
//      " \nstudents_reached\t"<<students_reached<<
//      " \nyr-mo-day "<<yr<<"-"<<mo<<"-"<<day<<
//      endl;
     
    //descent into 2nd file
    ctr2 = 0;
    matched = 0;
    
    rewind(f1);
    
    while (!feof (f1)&&matched==0){

     ctr2 += 1;
     //if (ctr2%100000==0) cout<<" reading outcomes; entry "<<ctr2<<endl;

     retval = fscanf(f1,"%50[^,],%50[^,],%50[^,],%50[^,],%lf,%lf,%30[^,],%10[^,],%d,%20[^,],%100[^,],%100[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%5[^,],%10[^,],%5[^,],%5[^,],%100[^,],%100[^,],%100[^,],%100[^,],%50[^,],%100[^,],%50[^,],%f,%f,%f,%d,%5[^,],%5[^,],%d-%d-%d",
     projectid_1,
	 teacher_acctid,
	 schoolid,
	 school_ncesid,
	 &school_latitude,
	 &school_longitude,
	 school_city,
	 school_state,
	 &school_zip,
	 school_metro,
	 school_district,
	 school_county,
	 school_charter,
	 school_magnet,
	 school_year_round,
	 school_nlns,
	 school_kipp,
	 school_charter_ready_promise,
	 teacher_prefix,
	 teacher_teach_for_america,
	 teacher_ny_teaching_fellow,
	 primary_focus_subject,
	 primary_focus_area,
	 secondary_focus_subject,
	 secondary_focus_area,
	 resource_type,
	 poverty_level,
	 grade_level,
	 &fulfillment_labor_materials,
	 &total_price_excluding_optional_support,
	 &total_price_including_optional_support,
	 &students_reached,
	 eligible_double_your_impact_match,
	 eligible_almost_home_match,
	 &yr,
	 &mo,
	 &day);

	 if (retval!=37) continue;

     //cout<<" pid 1, 2 "<<projectid_1<<" "<<projectid_2<<" ctr1 "<<ctr1<<" ctr2 "<<ctr2<<" retval "<<retval<<" school_latitude "<<school_latitude<<endl;//yr "<<yr<<" retval "<<retval<<endl;
     
      if (strcmp(projectid_1,projectid_2)==0){
        cout<<" project match! "<<endl;
        matched = 1;
        combined_file << projectid_1;
        combined_file << "\t";
        combined_file << teacher_acctid;
        combined_file << "\t";
	 	combined_file << schoolid;
        combined_file << "\t";
	 	combined_file << school_ncesid;
        combined_file << "\t";
	 	combined_file << school_latitude;
        combined_file << "\t";
	 	combined_file << school_longitude;
        combined_file << "\t";
	 	combined_file << school_city;
        combined_file << "\t";
	 	combined_file << school_state;
        combined_file << "\t";
	 	combined_file << school_zip;
        combined_file << "\t";
	 	combined_file << school_metro;
        combined_file << "\t";
	 	combined_file << school_district;
        combined_file << "\t";
	 	combined_file << school_county;
        combined_file << "\t";
	 	combined_file << school_charter;
        combined_file << "\t";
	 	combined_file << school_magnet;
        combined_file << "\t";
	 	combined_file << school_year_round;
        combined_file << "\t";
	 	combined_file << school_nlns;
        combined_file << "\t";
	 	combined_file << school_kipp;
        combined_file << "\t";
	 	combined_file << school_charter_ready_promise;
        combined_file << "\t";
	 	combined_file << teacher_prefix;
        combined_file << "\t";
	 	combined_file << teacher_teach_for_america;
        combined_file << "\t";
	 	combined_file << teacher_ny_teaching_fellow;
        combined_file << "\t";
	 	combined_file << primary_focus_subject;
        combined_file << "\t";
	 	combined_file << primary_focus_area;
        combined_file << "\t";
	 	combined_file << secondary_focus_subject;
        combined_file << "\t";
	 	combined_file << secondary_focus_area;
        combined_file << "\t";
	 	combined_file << resource_type;
        combined_file << "\t";
	 	combined_file << poverty_level;
        combined_file << "\t";
	 	combined_file << grade_level;
        combined_file << "\t";
	 	combined_file << fulfillment_labor_materials;
        combined_file << "\t";
	 	combined_file << total_price_excluding_optional_support;
        combined_file << "\t";
	 	combined_file << total_price_including_optional_support;
        combined_file << "\t";
	 	combined_file << students_reached;
        combined_file << "\t";
	 	combined_file << eligible_double_your_impact_match;
        combined_file << "\t";
	 	combined_file << eligible_almost_home_match;
        combined_file << "\t";
	 	combined_file << yr;
        combined_file << "\t";
	 	combined_file << mo;
        combined_file << "\t";
	 	combined_file << day;
        combined_file << "\t";

        combined_file << is_exciting,
        combined_file << "\t";
        combined_file << at_least_1_teacher_referred_donor,
        combined_file << "\t";
        combined_file << fully_funded,
        combined_file << "\t";
        combined_file << at_least_1_green_donation,
        combined_file << "\t";
        combined_file << great_chat,
        combined_file << "\t";
        combined_file << three_or_more_non_teacher_referred_donors,
        combined_file << "\t";
        combined_file << one_non_teacher_referred_donor_giving_100_plus,
        combined_file << "\t";
        combined_file << donation_from_thoughtful_donor,
        combined_file << "\t";
        combined_file << great_messages_proportion,
        combined_file << "\t";
        combined_file << teacher_referred_count,
        combined_file << "\t";
        combined_file << non_teacher_referred_count;

      }
    }
  }

}
