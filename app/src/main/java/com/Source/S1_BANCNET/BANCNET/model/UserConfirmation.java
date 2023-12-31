package com.Source.S1_BANCNET.BANCNET.model;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.PowerManager;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.Source.S1_BANCNET.BANCNET.CloseActivityClass;
import com.Source.S1_BANCNET.BANCNET.Main.MainActivity;
import com.Source.S1_BANCNET.BANCNET.R;
import com.Source.S1_BANCNET.BANCNET.SysApplication;

public class UserConfirmation extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    EditText etInputStr;
    Button buOK;
    Button buCancel;

    TextView textView01;
    TextView textView02;
    TextView textView03;
    TextView textView04;
    TextView textView05;

    TextView textView_txn;

    TextView textView_cur;

    Double dAmount;
    int inResult;
    String stResult;

    boolean androidThinking;
    public static String final_string;
    public static String input_type;

    private int inTimeOut = 20;

    protected PowerManager.WakeLock mWakeLock;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CloseActivityClass.activityList.add(this);
        SysApplication.getInstance().addActivity(this);
        super.onCreate(savedInstanceState);
        Bundle bundle = getIntent().getExtras();

        overridePendingTransition(0, 0); // disable the animation, faster
        
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);    //Tine:  Show Status Bar

        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        setContentView(R.layout.user_confirmation);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_BANCNET:AAA-ConfirmMenu>>WAKE_LOCK");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String dispmsg=intent.getStringExtra("pass_in_string");
        Log.i("saturn dispmsg", dispmsg);

        textView_txn = (TextView) findViewById(R.id.textView_txn);
        textView_cur = (TextView) findViewById(R.id.textView_cur);

        textView01 = (TextView) findViewById(R.id.msg_text_01);
        textView02 = (TextView) findViewById(R.id.msg_text_02);
        textView03 = (TextView) findViewById(R.id.msg_text_03);
        textView04 = (TextView) findViewById(R.id.msg_text_04);
        textView05 = (TextView) findViewById(R.id.msg_text_05);


        String[] dispmsginfo = dispmsg.split("\\|");
		int msgcnt = dispmsginfo.length;

		System.out.println("msgcnt [" + msgcnt + "]");

        for (int inIdx = 0 ; inIdx < msgcnt; inIdx++)
        {
            System.out.println("split msg [" + inIdx + "][" + dispmsginfo[inIdx] + "]");
            switch (inIdx)
            {
                case 0:
                    textView_txn.setText(dispmsginfo[inIdx]);
                    break;
                case 1:
                    textView01.setText(dispmsginfo[inIdx]);
                    break;
                case 2:
                    textView02.setText(dispmsginfo[inIdx]);
                    break;
                case 3:
                    textView03.setText(dispmsginfo[inIdx]);
                    break;
                case 4:
                    textView04.setText(dispmsginfo[inIdx]);
                    break;
                case 5:
                    textView05.setText(dispmsginfo[inIdx]);
                    break;
            }
        }

        buOK = (Button) findViewById(R.id.IPT_OKButton);
        buCancel = (Button) findViewById(R.id.IPT_CancelButton);

        /*Start timer*/
        getTimerRestart();

        buOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = "CONFIRM";
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(GetAmount.this,MainActivity.class));
                UserConfirmation.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buOK");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }

            }
        });

        buCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                final_string = "CANCEL";
                //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));
                UserConfirmation.this.finish();

                Log.i("PATRICK", "Get Amount KeyBoard buCancel");

                // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
                synchronized (MainActivity.LOCK) {
                    MainActivity.LOCK.setCondition(true);
                    MainActivity.LOCK.notifyAll();

                }
            }
        });

    }

    /**
     * 取消倒计时
     */
    public void getTimerCancel() {
        timer.cancel();
    }

    /**
     * 开始倒计时
     */
    public void getTimerRestart()
    {
        timer.start();
    }

    private CountDownTimer timer = new CountDownTimer(inTimeOut*1000, 1000) {

        @Override
        public void onTick(long millisUntilFinished) {
            Log.i("Timer", "saturn Timer onTick UserConfirmation");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "saturn Timer onFinish UserConfirmation");

            //etInputStr.clear();
            final_string = "TO";

            //startActivity(new Intent(ConfirmMenu.this,MainActivity.class));

            Log.i("PATRICK", "Timeout saturn UserConfirmation");
            UserConfirmation.this.finish();

            // do whatever you need in child activity, but once you want to finish, do this and continue in parent activity
            synchronized (MainActivity.LOCK) {
                MainActivity.LOCK.setCondition(true);
                MainActivity.LOCK.notifyAll();
            }
        }
    };

    @Override
    public void onBackPressed() {

    }

    @Override
    protected void onDestroy() {
        this.mWakeLock.release();
        super.onDestroy();
        final_string = null;
        mContext = null;
        SysApplication.getInstance().removeActivity(this);
        CloseActivityClass.activityList.remove(this);
    }

}

