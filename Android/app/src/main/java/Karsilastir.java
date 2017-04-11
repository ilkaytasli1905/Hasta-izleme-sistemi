package com.ilkaytasli.hastaizlemesistemi;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;

/**
 * Created by itasli on 24/12/16.
 */


public class Karsilastir extends Fragment {
    SQLiteDatabase db;
    Cursor c;
    String a,nemOrani,ortamSicakligi,vucutSicakligi,co,duman,nabiz;
    TextView sol1,sol2,sol3,sol4,sol5,sol6,sag1,sag2,sag3,sag4,sag5,sag6;
    Spinner solSpinner , sagSpinner;
    String sonAdres;
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.karsilastir, container, false);
        Bundle bundle = this.getArguments();
        db= getActivity().openOrCreateDatabase("HastaKayit", Context.MODE_PRIVATE,null);
        sonAdres = bundle.getString("sonIp", "Daha önce kullanılan bir ip adresi bulunamadı.");
        sol1 = (TextView)v.findViewById(R.id.textSol1);
        sol2 = (TextView)v.findViewById(R.id.textSol2);
        sol3 = (TextView)v.findViewById(R.id.textSol3);
        sol4 = (TextView)v.findViewById(R.id.textSol4);
        sol5 = (TextView)v.findViewById(R.id.textSol5);
        sol6 = (TextView)v.findViewById(R.id.textSol6);
        sag1 = (TextView)v.findViewById(R.id.textSag1);
        sag2 = (TextView)v.findViewById(R.id.textSag2);
        sag3 = (TextView)v.findViewById(R.id.textSag3);
        sag4 = (TextView)v.findViewById(R.id.textSag4);
        sag5 = (TextView)v.findViewById(R.id.textSag5);
        sag6 = (TextView)v.findViewById(R.id.textSag6);
        ArrayList<String> tarihler = ListeyiGetir();
        solSpinner = (Spinner)v.findViewById(R.id.spinner1);
        sagSpinner = (Spinner)v.findViewById(R.id.spinner2);
        ArrayAdapter<String> arrayAdapter = new ArrayAdapter<String>(getActivity(),
                R.layout.support_simple_spinner_dropdown_item, tarihler);
        solSpinner.setAdapter(arrayAdapter);
        sagSpinner.setAdapter(arrayAdapter);
        sagSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> parent, View view,
                                       int position, long id) {
                if(c != null)
                if(c.moveToLast()){
                    do{
                        String x = c.getString(1);
                        String k = parent.getItemAtPosition(position).toString();
                        if(x.equals(k))
                        {
                            sag1.setText( c.getString(2));
                            sag2.setText( c.getString(3));
                            sag3.setText( c.getString(4));
                            sag4.setText( c.getString(5));
                            sag5.setText( c.getString(6));
                            sag6.setText( c.getString(7));
                            String column9 = c.getString(7);
                            break;
                        }
                    }while(c.moveToPrevious());
                    db.close();
                }
            }
            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        solSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> parent, View view,
                                       int position, long id) {
                if(c != null)
                if(c.moveToLast()){
                    do{
                        String x = c.getString(1);
                        String k = parent.getItemAtPosition(position).toString();
                        if(x.equals(k))
                        {
                            sol1.setText( c.getString(2));
                            sol2.setText( c.getString(3));
                            sol3.setText( c.getString(4));
                            sol4.setText( c.getString(5));
                            sol5.setText( c.getString(6));
                            sol6.setText( c.getString(7));
                            String column9 = c.getString(7);
                            break;
                        }

                    }while(c.moveToPrevious());
                }



            }
            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        return v;
    }


    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        getActivity().setTitle("Hasta İzleme Sistemi - Karşılaştır");
    }

    public ArrayList<String> ListeyiGetir()
    {
        ArrayList<String> temp = new ArrayList<>();
        c = db.rawQuery("SELECT Ip,KayitTarihi,NemOrani,OdaSicakligi,VucutSicakligi,Karbonmonoksit,Duman,Nabiz,Nabiz FROM tbl_Hasta_Kayit WHERE Ip='"+sonAdres+"';",null);
        if(c.moveToLast()){
            do{
                temp.add(c.getString(1));
            }while(c.moveToPrevious());
        }
        return temp;
    }

}

