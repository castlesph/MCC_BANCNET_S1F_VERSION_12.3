package com.Source.S1_BANCNET.BANCNET.model;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Typeface;
import android.os.Bundle;
import android.content.Context;
import android.icu.math.BigDecimal;
import android.icu.text.NumberFormat;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.PowerManager;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.InputType;
import android.text.TextWatcher;
import android.text.method.PasswordTransformationMethod;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.Source.S1_BANCNET.BANCNET.CloseActivityClass;
import com.Source.S1_BANCNET.BANCNET.Main.MainActivity;
import com.Source.S1_BANCNET.BANCNET.R;
import com.Source.S1_BANCNET.BANCNET.SysApplication;
import com.Source.S1_BANCNET.BANCNET.Trans.DoneOnEditorActionListener;
import com.Source.S1_BANCNET.BANCNET.model.DemoAppActivity;

public class UserInputString extends DemoAppActivity {

    private Context mContext;

    private Button mButton;
    final Context c = this;

    EditText etInputStr;
    Button buOK;
    Button buCancel;
    TextView tvMessage;

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

        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

        setContentView(R.layout.get_string_main);

        /* This code together with the one in onDestroy()
         * will make the screen be always on until this Activity gets destroyed. */
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "S1_BANCNET:AAA-UserInputString");
        this.mWakeLock.acquire();

        Intent intent=getIntent();
        String value=intent.getStringExtra("pass_in_string");
        Log.i("pass in value", value);

        tvMessage = (TextView) findViewById(R.id.Input_MessageLabel);
        etInputStr = (EditText) findViewById(R.id.UserInput);

        input_type = value.substring(0, 1);
        if (input_type.equals("1")) {
            Log.i("setInputType", "TYPE_CLASS_NUMBER");
            etInputStr.setInputType(InputType.TYPE_CLASS_NUMBER);
        }

        if (input_type.equals("2")) {
            etInputStr.setSingleLine(true);
            Log.i("setInputType", "TYPE_NUMBER_VARIATION_PASSWORD");
            etInputStr.setInputType(InputType.TYPE_NUMBER_VARIATION_PASSWORD);
            //etInputStr.setInputType(InputType.TYPE_TEXT_VARIATION_WEB_PASSWORD);

        }

        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(etInputStr, InputMethodManager.SHOW_IMPLICIT);

        if (input_type.equals("2"))
        {
            Log.i("setTransformationMethod", "PasswordTransformationMethod");
            etInputStr.setTypeface(Typeface.DEFAULT);
            etInputStr.setTransformationMethod(new PasswordTransformationMethod());
            //明文显示密码
            //edittext.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
            //隐藏密码
            etInputStr.setTransformationMethod(PasswordTransformationMethod.getInstance());

            etInputStr.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_VARIATION_PASSWORD);
        }
        else
            etInputStr.setTransformationMethod(null); //set android:inputType="numberPassword" in xml and set this to display numeric only

        etInputStr.setOnEditorActionListener(new DoneOnEditorActionListener());
        etInputStr.setSelection(etInputStr.getText().length());

        buOK = (Button) findViewById(R.id.IPT_OKButton);
        buCancel = (Button) findViewById(R.id.IPT_CancelButton);

        /*Start timer*/
        getTimerRestart();

        etInputStr.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

                Log.i("onTextChanged", "onTextChanged reset timer");
                /*reset timer*/
                getTimerCancel(); getTimerRestart();
            }

            @Override
            public void afterTextChanged(Editable editable) {
                if (etInputStr == null) return;


                String inputString = editable.toString();
                etInputStr.removeTextChangedListener(this);
                String cleanString = inputString.toString().replaceAll("[$,.]", "");

                //BigDecimal bigDecimal = new BigDecimal(cleanString).setScale(2, BigDecimal.ROUND_FLOOR).divide(new BigDecimal(100), BigDecimal.ROUND_FLOOR);
                //String  converted = NumberFormat.getCurrencyInstance().format(bigDecimal);

                etInputStr.setText(cleanString);
                etInputStr.setSelection(cleanString.length());
                etInputStr.addTextChangedListener(this);

            }
        });

        buOK.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                /*cancel timer first*/
                getTimerCancel();

                //dAmount = Double.parseDouble(etAmount.getText().toString());
                final_string = etInputStr.getText().toString();
                //final_string = final_string.replaceAll("[$,.]", "");

                //startActivity(new Intent(GetAmount.this,MainActivity.class));
                UserInputString.this.finish();

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
                startActivity(new Intent(UserInputString.this,MainActivity.class));
                UserInputString.this.finish();

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
            Log.i("Timer", "Timer onTick");
        }

        @Override
        public void onFinish() {
            Log.i("Timer", "Timer onFinish");

            //etInputStr.clear();
            final_string = "TO";

            startActivity(new Intent(UserInputString.this,MainActivity.class));

            Log.i("PATRICK", "Timeout UserInputString");
            UserInputString.this.finish();

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

