package com.ilkaytasli.hastaizlemesistemi;
        import android.content.Context;
        import android.database.Cursor;
        import android.database.sqlite.SQLiteDatabase;
        import android.graphics.Bitmap;
        import android.graphics.Paint;
        import android.os.Bundle;
        import android.os.Environment;
        import android.support.annotation.Nullable;
        import android.support.design.widget.FloatingActionButton;
        import android.support.v4.app.Fragment;
        import android.view.LayoutInflater;
        import android.view.View;
        import android.view.ViewGroup;
        import org.achartengine.ChartFactory;
        import org.achartengine.chart.PointStyle;
        import org.achartengine.model.XYMultipleSeriesDataset;
        import org.achartengine.model.XYSeries;
        import org.achartengine.renderer.XYMultipleSeriesRenderer;
        import org.achartengine.renderer.XYSeriesRenderer;
        import android.app.Activity;
        import android.graphics.Color;
        import android.os.Bundle;
        import android.view.Menu;
        import android.view.View;
        import android.widget.LinearLayout;
        import android.widget.TabHost;
        import android.widget.Toast;
        import android.widget.TabHost.TabSpec;

        import java.io.ByteArrayOutputStream;
        import java.io.File;
        import java.io.FileOutputStream;
        import java.io.IOException;
        import java.text.SimpleDateFormat;
        import java.util.ArrayList;
        import java.util.Calendar;

/**
 * Created by itasli on 24/12/16.
 */


public class Grafik extends Fragment implements View.OnClickListener  {
    View v,mainViev;
    private View mChart , eChart , kChart;
    SQLiteDatabase db;
    TabHost tabHost;
    TabSpec tab1, tab2, tab3;
    Cursor c ;
    String sonAdres;
    FloatingActionButton fabSave;
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        v = inflater.inflate(R.layout.grafik, container, false);
        mainViev = v;
        Bundle bundle = this.getArguments();
        sonAdres = bundle.getString("sonIp", "Daha önce kullanılan bir ip adresi bulunamadı.");
        fabSave = (FloatingActionButton)v.findViewById(R.id.save);
        fabSave.setOnClickListener(this);
        try {


            tabHost = (TabHost)v.findViewById(android.R.id.tabhost);
            tabHost.setup();

            tab1 = tabHost.newTabSpec("tab1");
            tab1.setIndicator("1 Aylık")
                    .setContent(R.id.line1);
            tab2 = tabHost.newTabSpec("tab2");
            tab2.setIndicator("3 Aylık").setContent(R.id.line2);

            tab3 = tabHost.newTabSpec("tab3");
            tab3.setIndicator("6 Aylık").setContent(R.id.line3);

            tabHost.addTab(tab1);
            tabHost.addTab(tab2);
            tabHost.addTab(tab3);
            GrafikHazirla(-30,1);
            GrafikHazirla(-90,2);
            GrafikHazirla(-180,3);
            tabHost.setCurrentTab(0);
        } catch (Exception e) {
            Toast.makeText(getActivity()
                    , e.getMessage(), Toast.LENGTH_LONG).show();
        }
        return v;
    }


    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        //you can set the title for your toolbar here for different fragments different titles
        getActivity().setTitle("Hasta İzleme Sistemi - Grafik");
    }

    public void GrafikHazirla(int gun , int deger)
    {
        XYSeries nemSeries = new XYSeries("Nem");
        XYSeries ortamSeries = new XYSeries("Ortam Sıcaklığı");
        XYSeries vucutSeries = new XYSeries("Vücut Sıcaklığı");
        XYSeries coSeries = new XYSeries("Karbonmonoksit");
        XYSeries dumanSeries = new XYSeries("Duman");
        XYSeries nabizSeries = new XYSeries("Nabız");
        Calendar cal = Calendar.getInstance();
        cal.add(Calendar.DAY_OF_MONTH,gun);
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String formattedDate = df.format(cal.getTime());
        XYMultipleSeriesRenderer multiRenderer = new XYMultipleSeriesRenderer();
        db= getActivity().openOrCreateDatabase("HastaKayit", Context.MODE_PRIVATE,null);
        c = db.rawQuery("SELECT Ip,KayitTarihi,NemOrani,OdaSicakligi,VucutSicakligi,Karbonmonoksit,Duman,Nabiz,Nabiz FROM tbl_Hasta_Kayit WHERE Ip='"+sonAdres+"'AND KayitTarihi>'"+ formattedDate+ "';",null);
        int sayac = 1;
        if(c.moveToFirst()){
            do{     multiRenderer.addXTextLabel(sayac,c.getString(1));
                    nemSeries.add(sayac,Float.parseFloat(c.getString(2)));
                    ortamSeries.add(sayac,Float.parseFloat(c.getString(3)));
                    vucutSeries.add(sayac,Float.parseFloat(c.getString(4)));
                    coSeries.add(sayac,Float.parseFloat(c.getString(5)));
                    dumanSeries.add(sayac,Float.parseFloat(c.getString(6)));
                    nabizSeries.add(sayac,Float.parseFloat(c.getString(7)));
                    String column9 = c.getString(7);
                    sayac++;
            }while(c.moveToNext());}
        c.close();
        db.close();
        XYMultipleSeriesDataset dataset = new XYMultipleSeriesDataset();
        dataset.addSeries(nemSeries);
        dataset.addSeries(ortamSeries);
        dataset.addSeries(vucutSeries);
        dataset.addSeries(coSeries);
        dataset.addSeries(dumanSeries);
        dataset.addSeries(nabizSeries);
        XYSeriesRenderer nemRenderer = new XYSeriesRenderer();
            nemRenderer.setColor(Color.BLUE);
                nemRenderer.setPointStyle(PointStyle.POINT);
            nemRenderer.setFillPoints(true);
            nemRenderer.setLineWidth(5);
            nemRenderer.setDisplayChartValues(true);
            nemRenderer.setChartValuesTextSize(50);
        XYSeriesRenderer ortamRenderer = new XYSeriesRenderer();
            ortamRenderer.setColor(Color.GREEN);
            ortamRenderer.setPointStyle(PointStyle.POINT);
            ortamRenderer.setFillPoints(true);
            ortamRenderer.setLineWidth(5);
            ortamRenderer.setDisplayChartValues(true);
            ortamRenderer.setChartValuesTextSize(50);
        XYSeriesRenderer vucutRenderer = new XYSeriesRenderer();
            vucutRenderer.setColor(Color.RED);
            vucutRenderer.setPointStyle(PointStyle.POINT);
            vucutRenderer.setFillPoints(true);
            vucutRenderer.setLineWidth(5);
            vucutRenderer.setDisplayChartValues(true);
            vucutRenderer.setChartValuesTextSize(50);
        XYSeriesRenderer coRenderer = new XYSeriesRenderer();
            coRenderer.setColor(Color.YELLOW);
            coRenderer.setPointStyle(PointStyle.POINT);
            coRenderer.setFillPoints(true);
            coRenderer.setLineWidth(5);
            coRenderer.setDisplayChartValues(true);
            coRenderer.setChartValuesTextSize(50);
        XYSeriesRenderer dumanRenderer = new XYSeriesRenderer();
            dumanRenderer.setColor(Color.CYAN);
            dumanRenderer.setPointStyle(PointStyle.POINT);
            dumanRenderer.setFillPoints(true);
            dumanRenderer.setLineWidth(5);
            dumanRenderer.setDisplayChartValues(true);
            dumanRenderer.setChartValuesTextSize(50);
        XYSeriesRenderer nabizRenderer = new XYSeriesRenderer();
            nabizRenderer.setColor(Color.MAGENTA);
            nabizRenderer.setPointStyle(PointStyle.POINT);
            nabizRenderer.setFillPoints(true);
            nabizRenderer.setLineWidth(5);
            nabizRenderer.setDisplayChartValues(true);
            nabizRenderer.setChartValuesTextSize(50);

        multiRenderer.setChartTitle("\n\nHastaya ait nem,vücut sıcaklığı ve nabız değerleri yanında \n ortam sıcaklığı ve ortamdaki karbonmonoksit ve durman\n miktarının değişimi.");
        multiRenderer.setChartTitleTextSize(50);
        multiRenderer.setGridColor(Color.CYAN);
        multiRenderer.setXLabels(0);
        multiRenderer.setAxisTitleTextSize(50);
        multiRenderer.setXTitle("Tarihler");
        multiRenderer.setYTitle("Değerler");
        multiRenderer.setMargins(new int[]{200, 75, 175, 5});
        multiRenderer.setXLabelsAlign(Paint.Align.CENTER);
        multiRenderer.setYLabelsAlign(Paint.Align.LEFT);
        multiRenderer.setZoomButtonsVisible(false);
        multiRenderer.setLegendTextSize(40);
        multiRenderer.setLegendHeight(30);
        multiRenderer.setFitLegend(true);
        multiRenderer.addSeriesRenderer(nemRenderer);
        multiRenderer.addSeriesRenderer(ortamRenderer);
        multiRenderer.addSeriesRenderer(vucutRenderer);
        multiRenderer.addSeriesRenderer(coRenderer);
        multiRenderer.addSeriesRenderer(dumanRenderer);
        multiRenderer.addSeriesRenderer(nabizRenderer);
        multiRenderer.setLabelsTextSize(50);
        if(deger == 1)
        {
            LinearLayout chartContainer = (LinearLayout) v.findViewById(R.id.line1);
            mChart = ChartFactory.getLineChartView(getActivity().getBaseContext(), dataset, multiRenderer);
            chartContainer.addView(mChart);
        }
        if(deger == 2)
        {
            LinearLayout chartContainer = (LinearLayout) v.findViewById(R.id.line2);
            kChart = ChartFactory.getLineChartView(getActivity().getBaseContext(), dataset, multiRenderer);
            chartContainer.addView(kChart);
        }
        if(deger == 3)
        {
            LinearLayout chartContainer = (LinearLayout) v.findViewById(R.id.line3);
            eChart = ChartFactory.getLineChartView(getActivity().getBaseContext(), dataset, multiRenderer);
            chartContainer.addView(eChart);
        }
    }

    @Override
    public void onClick(View v) {
        MainActivity ma = (MainActivity)getActivity();
        ma.takeScreenShot();
        Toast.makeText(getActivity(),"Ekran Görüntüsü başarı ile alındı.",Toast.LENGTH_LONG).show();
    }
}

