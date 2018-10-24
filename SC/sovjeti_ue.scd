(
s.boot;
s.doWhenBooted{
    forkIfNeeded{

        //free buffers if already allocated
        if(topEnvironment.includesKey('filee'), {
            topEnvironment.at('filee').free;
        });
        s.sync;

        // populate buffers
        topEnvironment.put(
            'filee', Buffer.read(s, "X:/lydfiler/TREE_KNIRK.wav")
        );
        s.sync;

        "buffers loaded".postln;
    };

    MIDIIn.connectAll;
    fork{
        [
            \cc_1,
            \cc_2,
            \cc_3,
            \cc_4,
            \cc_5,
            \cc_6,
            \cc_7,
            \cc_8,
            \cc_9,
            \cc_10,
            \cc_11,
            \cc_12,
            \cc_13,
            \cc_14,
            \cc_15,
            \cc_16,
            \cc_17,
            \cc_18,
            \cc_19,
            \cc_20,
            \cc_21,
            \cc_22,
            \cc_23,
            \cc_24,
            \cc_25,
            \cc_26,
            \cc_27,
            \cc_28,
            \cc_29,
            \cc_30,
            \cc_31,
            \cc_32,
            \cc_33,
            \cc_34,
            \cc_35,
            \cc_36,
            \cc_37,
            \cc_38,
            \cc_39,
            \cc_40
        ].do({arg paramName, i;
            var path = "/midi/%".format(paramName).asSymbol;
            Ndef(path, 0.0).kr(1);
            MIDIdef.cc("cc_%Responder".format(paramName).asSymbol, {arg val, num, chan, src;
                Ndef(path).bus.set(val);
            },
            ccNum: 1+i,
            chan: 0
            );
        });

        [
            \button_1,
            \button_2,
            \button_3,
            \button_4,
            \button_5,
            \button_6,
            \button_7,
            \button_8,
            \button_9,
            \button_10,
            \button_11,
            \button_12,
            \button_13,
            \button_14,
            \button_15,
            \button_16,
            \button_17,
            \button_18,
            \button_19,
            \button_20
        ].do({arg paramName, i;
            var value;
            var path = "/midi/%".format(paramName).asSymbol;
            Ndef(path, 0.0).kr(1);

            MIDIdef.noteOn("noteOn_%Responder".format(paramName).asSymbol, {arg velocity, note, chan, src;
                value = 1;
                Ndef(path).bus.set(value);
            },
            noteNum: 48+i,
            chan: 0
            );

            MIDIdef.noteOff("noteOff_%Responder".format(paramName).asSymbol, {arg velocity, note, chan, src;
                value = 0;
                Ndef(path).bus.set(value);

            },
            noteNum: 48+i,
            chan: 0
            );


        });

    };

    /////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

    //page A
    // row one

    Ndef(\knirk, {arg bufnum, numChannels;
        var sig, mod, play1, play2, play3;
        var rate = Ndef.kr('/midi/cc_1').linlin(0,127,0.01,2.0);
        var trspeed = Ndef.kr('/midi/cc_6').linlin(0,127,0.0,5.0);
        var moth = Ndef.kr('/midi/cc_11').linlin(0,127,0.0,1.0);
        var amp = Ndef.kr('/midi/cc_16').linlin(0,127,0.0,1.0);
        var switch = Ndef.kr('/midi/button_6');

        var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
        var trigger= Impulse.kr(1.4);
        bufnum = topEnvironment['filee'].bufnum;
        numChannels = topEnvironment['filee'].numChannels;

        play1=PlayBufCF.ar(
            numChannels,
            bufnum,
            BufRateScale.kr(bufnum)*rate,
            Impulse.kr(trspeed),//trigger,
            startPos, loop:1, lag: 0.3) * 4;
        play2=PlayBufCF.ar(
            numChannels,
            bufnum,
            BufRateScale.kr(bufnum)*rate * 0.95,
            Impulse.kr(trspeed),//trigger,
            startPos, loop:1, lag: 0.3) * 4;
        play3=PlayBufCF.ar(
            numChannels,
            bufnum,
            BufRateScale.kr(bufnum)*rate * 0.9,
            Impulse.kr(trspeed),//trigger,
            startPos, loop:1, lag: 0.3) * 4;

        sig=play1+play2+play3;
        sig=sig.clip(-0.99,0.99);
        sig*amp*switch;
    }).playN([0,1]);

	// row two
	Ndef(\fice, {
        var dur, freqs, denses, snd, squi, sum;
        var dens = Ndef.kr('/midi/cc_2').linlin(0,127,0.0,2.0);
        var durMax = Ndef.kr('/midi/cc_7').linlin(0,127,0.01,5.0);
        var pulse = Ndef.kr('/midi/cc_12').linlin(0,127,0.05,5.0);
        var ficAmp = Ndef.kr('/midi/cc_17').linlin(0,127,0.0,2.0);
        var switch = Ndef.kr('/midi/button_7');
        freqs = [100,200,300,400,500]*Pulse.ar(pulse)*100;
        dur=SinOsc.ar(2).range(2,durMax);
        denses= [dens,dens*1.1,dens*1.2,dens*1.3,dens*1.4];
        snd=Splay.ar(
            SinGrain.ar(Dust.ar(denses), dur, freqs, 0.1),
            LFNoise0.kr(1).range(0,1).lag(0.4);
        );
        squi=Squiz.ar(snd, 4.0, 1.0, 10.0);
        sum=squi+LPF.ar(squi, 100, 2);
        sum=(sum*3*ficAmp*switch).clip(-0.99,0.99);
		sum = Limiter.ar(sum, 0.99);
		LeakDC.ar(sum);
    }).playN([0,1]);

	// row three
	Ndef(\ficerytm, {
        var dur, freqs, denses, snd, squi, sum;
        var dens = Ndef.kr('/midi/cc_3').linlin(0,127,0.0,2.0);
        var durMax = Ndef.kr('/midi/cc_8').linlin(0,127,0.01,5.0);
        var pulse = Ndef.kr('/midi/cc_13').linlin(0,127,0.05,5.0);

		var pulselo = Ndef.kr('/midi/cc_4').linlin(0,127,0.0001,0.499);
		var pulsehi = Ndef.kr('/midi/cc_9').linexp(0,127,0.5,30.0);
		var pulsechangespeed = Ndef.kr('/midi/cc_14').linlin(0,127,0.01,3.0);

        var ficAmp = Ndef.kr('/midi/cc_18').linlin(0,127,0.0,1.0);
        var switch = Ndef.kr('/midi/button_8');
		freqs = [100,200,300,400,500]*Pulse.ar(pulse)*100;
        dur=SinOsc.ar(2).range(2,durMax);
        denses= [dens,dens*1.1,dens*1.2,dens*1.3,dens*1.4];
        snd=Splay.ar(
            SinGrain.ar(Dust.ar(denses), dur, freqs, 0.1),
            LFNoise0.kr(1).range(0,1).lag(0.4);
        );
        squi=Squiz.ar(snd, 4.0, 1.0, 10.0);
        sum=squi+LPF.ar(squi, 100, 2);
		sum = Pulse.ar(LFNoise0.kr(pulsechangespeed).abs.linlin(0.0,1.0,pulselo,pulsehi))*sum;
        sum=(sum*ficAmp*switch).clip(-0.99,0.99);
		sum = CompanderD.ar(sum,0.5,1,1/3);
		sum = LeakDC.ar(sum);
		sum = Limiter.ar(sum, 0.99);
		sum;
    }).playN([0,1]);


    //"done loading".postln;
};
)