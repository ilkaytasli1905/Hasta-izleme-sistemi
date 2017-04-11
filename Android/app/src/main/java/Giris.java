package com.ilkaytasli.hastaizlemesistemi;
import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

/**
 * Created by itasli on 24/12/16.
 */


public class Giris extends Fragment implements View.OnClickListener {
    View mainView;
    Button bYeniAdres;
    Button bSonAdres;
    Fragment nextFrag;
    Bundle args;
    String sonAdres;
    SQLiteDatabase db;
    EditText a;
    TextView b;
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        mainView =  inflater.inflate(R.layout.giris, container, false);
        bYeniAdres = (Button) mainView.findViewById(R.id.bYeniAdres);
        bSonAdres = (Button) mainView.findViewById(R.id.bSonAdres);
        a = (EditText)mainView.findViewById(R.id.editText1);
        b = (TextView)mainView.findViewById(R.id.editText2);
        VeriTabanıKontrol();
        Bundle bundle = this.getArguments();
        sonAdres = bundle.getString("sonIp", "Daha önce kullanılan bir ip adresi bulunamadı.");
        b.setText("Son Kullanılan Ip = " + sonAdres);
        if(sonAdres=="Ip Bulunamadi.")
        {
           bSonAdres.setEnabled(false);
        }
        else bSonAdres.setOnClickListener(this);
        bYeniAdres.setOnClickListener(this);
        return mainView;
    }
    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        //you can set the title for your toolbar here for different fragments different titles
        getActivity().setTitle("Hasta İzleme Sistemi - Giriş");
    }

    @Override
    public void onClick(View v) {
        switch(v.getId()){
            case R.id.bYeniAdres:

                    if(a.getText().toString().equals(""))
                    {
                        Toast.makeText(getActivity(),"Lütfen bir adres giriniz.",Toast.LENGTH_LONG);
                    }
                    else{
                    nextFrag = new Izle();
                    args = new Bundle();
                    args.putString("sonIp", a.getText().toString());
                    nextFrag.setArguments(args);
                    FragmentTransaction ft = getFragmentManager().beginTransaction();
                    ft.replace(R.id.content_frame,nextFrag);
                    ft.addToBackStack(null);
                    ft.commit();
                    DosyayaYaz(a.getText().toString());
                    }
                    break;
            case R.id.bSonAdres:
                    nextFrag = new Izle();
                    args = new Bundle();
                    args.putString("sonIp", sonAdres);
                    nextFrag.setArguments(args);
                    FragmentTransaction fta = getFragmentManager().beginTransaction();
                    fta.replace(R.id.content_frame,nextFrag);
                    fta.addToBackStack(null);
                    fta.commit();
                    break;
        }
    }
    public void DosyayaYaz(String ip)
    {
        try {
            FileOutputStream fileout = getActivity().openFileOutput("IpAdress.txt", Context.MODE_PRIVATE);
            OutputStreamWriter outputWriter = new OutputStreamWriter(fileout);
            outputWriter.write(ip);
            outputWriter.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void VeriTabanıKontrol()
    {
        db= getActivity().openOrCreateDatabase("HastaKayit", Context.MODE_PRIVATE,null);
        try {
            final String CREATE_TABLE_CONTAIN = "CREATE TABLE IF NOT EXISTS tbl_Hasta_Kayit ("
                    + "Id INTEGER primary key AUTOINCREMENT,"
                    + "Ip TEXT,"
                    + "KayitTarihi DATETIME,"
                    + "NemOrani TEXT,"
                    + "OdaSicakligi TEXT,"
                    + "VucutSicakligi TEXT,"
                    + "Karbonmonoksit TEXT,"
                    + "Duman TEXT,"
                    + "Nabiz TEXT);";
            db.execSQL(CREATE_TABLE_CONTAIN);
        }
        catch (Exception e)
        {
            Toast.makeText(getActivity(), "Veri Tabanı bağlantısında problem oluştu."+e.toString(), Toast.LENGTH_LONG).show();
            getActivity().finish();
        }
    }
}

