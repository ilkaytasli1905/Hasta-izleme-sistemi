package com.ilkaytasli.hastaizlemesistemi;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.SystemClock;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Toast;

import org.jsoup.Connection;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * Created by itasli on 24/12/16.
 */


public class Izle extends Fragment implements View.OnClickListener {
    public WebView mWebView ;
    FloatingActionButton refreshButton;
    FloatingActionButton saveButton;
    String a,nemOrani,ortamSicakligi,vucutSicakligi,co,duman,nabiz;
    SQLiteDatabase db;
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.izle, container, false);
        mWebView = (WebView)v.findViewById(R.id.webView);
        refreshButton = (FloatingActionButton)v.findViewById(R.id.refresh);
        saveButton = (FloatingActionButton)v.findViewById(R.id.save);
        Bundle bundle = this.getArguments();
        a = bundle.getString("sonIp", "www.google.com");
        mWebView.loadUrl("http://" + a +"/");
        mWebView.getSettings().setJavaScriptEnabled(true);
        mWebView.setWebViewClient(new WebViewClient());
        refreshButton.setOnClickListener(this);
        saveButton.setOnClickListener(this);
        return v;
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        getActivity().setTitle("Hasta İzleme Sistemi - Anlık İzleme");
    }

    @Override
    public void onClick(View v) {
        switch(v.getId())
        {
            case R.id.refresh:
               // mWebView.loadUrl("http://" + a +"/");
                db= getActivity().openOrCreateDatabase("HastaKayit", Context.MODE_PRIVATE ,null);
                String sql="DELETE FROM tbl_Hasta_Kayit WHERE Duman = 'null'";
                db.execSQL(sql);
                db.close();
                break;
            case R.id.save:
                new getData().execute();
                break;
        }
    }

    private class getData extends AsyncTask<Void, Void, String> {
        @Override
        protected String doInBackground(Void...voids) {
            Document doc = null;
            db= getActivity().openOrCreateDatabase("HastaKayit", Context.MODE_PRIVATE ,null);
            try {
                doc = Jsoup.connect("http://"+ a +"/verial").timeout(10*1000).get();
                Elements elm = doc.select("p");
                int sayac = 0;
                for (Element el : elm) {
                    if(sayac == 0)
                    nemOrani = el.text();
                    if(sayac == 1)
                    ortamSicakligi = el.text();
                    if(sayac == 2)
                    vucutSicakligi = el.text();
                    if(sayac == 3)
                    co = el.text();
                    if(sayac == 4)
                    duman = el.text();
                    if(sayac == 5)
                    nabiz = el.text();
                    sayac ++;
                }
                Calendar c = Calendar.getInstance();
                SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                String formattedDate = df.format(c.getTime());
                String sql = "INSERT or replace INTO tbl_Hasta_Kayit (Ip,KayitTarihi,NemOrani,OdaSicakligi,VucutSicakligi,Karbonmonoksit,Duman,Nabiz) VALUES('"+a+"','"+formattedDate.toString()+"','"+nemOrani+"','"+ortamSicakligi+"','"+vucutSicakligi+"','"+co+"','"+duman+"','"+nabiz+"')" ;
                db.execSQL(sql);
                db.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return a;
        }
        @Override
        protected void onPostExecute(String result) {

        }
    }
}


