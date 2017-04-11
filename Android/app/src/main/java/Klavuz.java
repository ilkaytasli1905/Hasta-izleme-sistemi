package com.ilkaytasli.hastaizlemesistemi;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TabHost;
import android.widget.Toast;

/**
 * Created by itasli on 24/12/16.
 */


public class Klavuz extends Fragment implements View.OnClickListener{

    View v;
    ImageView iv ;
    FloatingActionButton fabPrevious , fabNext;
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        v = inflater.inflate(R.layout.klavuz, container, false);
        iv = (ImageView)v.findViewById(R.id.resim);
        fabPrevious = (FloatingActionButton)v.findViewById(R.id.fabSol);
        fabNext = (FloatingActionButton)v.findViewById(R.id.fabSag);
        iv.setImageResource(R.drawable.klavuz1);
        iv.setTag(R.drawable.klavuz1);
        fabPrevious.setVisibility(View.GONE);
        fabPrevious.setOnClickListener(this);
        fabNext.setOnClickListener(this);
        return v;
    }


    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        //you can set the title for your toolbar here for different fragments different titles
        getActivity().setTitle("Hasta İzleme Sistemi-Kılavuz");
    }
    @Override
    public void onClick(View v) {

        Integer a = (Integer)iv.getTag();
        switch(v.getId())
        {
            case R.id.fabSol:
                switch ((Integer)iv.getTag())
                {
                    case R.drawable.klavuz2:
                        iv.setImageResource(R.drawable.klavuz1);
                        iv.setTag(R.drawable.klavuz1);
                        fabPrevious.setVisibility(View.GONE);
                        break;
                    case R.drawable.klavuz3:
                        iv.setImageResource(R.drawable.klavuz2);
                        iv.setTag(R.drawable.klavuz2);
                        break;
                    case R.drawable.klavuz4:
                        iv.setImageResource(R.drawable.klavuz3);
                        iv.setTag(R.drawable.klavuz3);
                        break;
                    case R.drawable.klavuz5:
                        iv.setImageResource(R.drawable.klavuz4);
                        iv.setTag(R.drawable.klavuz4);
                        break;
                    case R.drawable.klavuz6:
                        iv.setImageResource(R.drawable.klavuz5);
                        iv.setTag(R.drawable.klavuz5);
                        break;
                    case R.drawable.klavuz7:
                        iv.setImageResource(R.drawable.klavuz6);
                        iv.setTag(R.drawable.klavuz6);
                        break;
                    case R.drawable.klavuz8:
                        iv.setImageResource(R.drawable.klavuz7);
                        iv.setTag(R.drawable.klavuz7);
                        break;
                    case R.drawable.klavuz9:
                        iv.setImageResource(R.drawable.klavuz8);
                        iv.setTag(R.drawable.klavuz8);
                        break;
                    case R.drawable.klavuz10:
                        iv.setImageResource(R.drawable.klavuz9);
                        iv.setTag(R.drawable.klavuz9);
                        break;
                    case R.drawable.klavuz11:
                        iv.setImageResource(R.drawable.klavuz10);
                        iv.setTag(R.drawable.klavuz10);
                        break;
                    case R.drawable.klavuz12:
                        iv.setImageResource(R.drawable.klavuz11);
                        iv.setTag(R.drawable.klavuz11);
                        fabNext.setVisibility(View.VISIBLE);
                        break;
                }
                break;
            case R.id.fabSag:
                switch ((Integer) iv.getTag())
                {
                    case R.drawable.klavuz1:
                        iv.setImageResource(R.drawable.klavuz2);
                        iv.setTag(R.drawable.klavuz2);
                        fabPrevious.setVisibility(View.VISIBLE);
                        break;
                    case R.drawable.klavuz2:
                        iv.setImageResource(R.drawable.klavuz3);
                        iv.setTag(R.drawable.klavuz3);
                        break;
                    case R.drawable.klavuz3:
                        iv.setImageResource(R.drawable.klavuz4);
                        iv.setTag(R.drawable.klavuz4);
                        break;
                    case R.drawable.klavuz4:
                        iv.setImageResource(R.drawable.klavuz5);
                        iv.setTag(R.drawable.klavuz5);
                        break;
                    case R.drawable.klavuz5:
                        iv.setImageResource(R.drawable.klavuz6);
                        iv.setTag(R.drawable.klavuz6);
                        break;
                    case R.drawable.klavuz6:
                        iv.setImageResource(R.drawable.klavuz7);
                        iv.setTag(R.drawable.klavuz7);
                        break;
                    case R.drawable.klavuz7:
                        iv.setImageResource(R.drawable.klavuz8);
                        iv.setTag(R.drawable.klavuz8);
                        break;
                    case R.drawable.klavuz8:
                        iv.setImageResource(R.drawable.klavuz9);
                        iv.setTag(R.drawable.klavuz9);
                        break;
                    case R.drawable.klavuz9:
                        iv.setImageResource(R.drawable.klavuz10);
                        iv.setTag(R.drawable.klavuz10);
                        break;
                    case R.drawable.klavuz10:
                        iv.setImageResource(R.drawable.klavuz11);
                        iv.setTag(R.drawable.klavuz11);
                        break;
                    case R.drawable.klavuz11:
                        iv.setImageResource(R.drawable.klavuz12);
                        iv.setTag(R.drawable.klavuz12);
                        fabNext.setVisibility(View.GONE);
                        break;
                }
                break;
        }

    }
}

