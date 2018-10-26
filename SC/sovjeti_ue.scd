(
fork{
	forkIfNeeded{

		//free buffers if already allocated
		if(topEnvironment.includesKey('filee'), {
			topEnvironment.at('filee').free;
		});
		s.sync;


		if(topEnvironment.includesKey('birds'), {
			topEnvironment.at('birds').free;
		});
		s.sync;

		if(topEnvironment.includesKey('frogs'), {
			topEnvironment.at('frogs').free;
		});
		s.sync;

		if(topEnvironment.includesKey('somer'), {
			topEnvironment.at('somer').free;
		});
		s.sync;

		if(topEnvironment.includesKey('klikbas'), {
			topEnvironment.at('klikbas').free;
		});
		s.sync;

		if(topEnvironment.includesKey('oita'), {
			topEnvironment.at('oita').free;
		});
		s.sync;

		if(topEnvironment.includesKey('snappa'), {
			topEnvironment.at('snappa').free;
		});
		s.sync;

		if(topEnvironment.includesKey('fish'), {
			topEnvironment.at('fish').free;
		});
		s.sync;

		if(topEnvironment.includesKey('sdr_trak'), {
			topEnvironment.at('sdr_trak').free;
		});
		s.sync;

		if(topEnvironment.includesKey('sdr_dron'), {
			topEnvironment.at('sdr_dron').free;
		});
		s.sync;

		if(topEnvironment.includesKey('sdr_ry'), {
			topEnvironment.at('sdr_ry').free;
		});
		s.sync;

		// populate buffers
		topEnvironment.put(
			'filee', Buffer.read(s, "X:/lydfiler/TREE_KNIRK.wav")
		);
		s.sync;

		topEnvironment.put(
			'birds', Buffer.read(s, "X:/salkin mada nature/tannatemplodebirdpitch.wav")
		);
		s.sync;

		topEnvironment.put(
			'frogs', Buffer.read(s, "X:/lydfiler/rams.wav")
		);
		s.sync;

		topEnvironment.put(
			'somer', Buffer.read(s, "X:/lydfiler/ssc-silent.wav")
		);
		s.sync;

		topEnvironment.put(
			'klikbas', Buffer.read(s, "X:/lydfiler/clikkibas.wav")
		);
		s.sync;

		topEnvironment.put(
			'oita', Buffer.read(s, "X:/lydfiler/natali/begie wood.wav")
		);
		s.sync;

		topEnvironment.put(
			'snappa', Buffer.read(s, "X:/lydfiler/0002 1-Audio_c1.wav")
		);
		s.sync;

		topEnvironment.put(
			'fish', Buffer.read(s, "X:/lydfiler/insekt 1-Audio_c1_remove_silence.wav");
		);
		s.sync;

		topEnvironment.put(
			'sdr_trak', Buffer.read(s, "X:/lydfiler/SDR_trak.wav")
		);
		s.sync;

		topEnvironment.put(
			'sdr_dron', Buffer.read(s, "X:/lydfiler/SDR_dron.wav")
		);
		s.sync;


		topEnvironment.put(
			'sdr_ry', Buffer.read(s, "X:/lydfiler/SDR_ryth.wav")
		);
		s.sync;

		"buffers loaded".postln;
	};

	MIDIIn.connectAll;
	fork{
		80.collect({arg i, item; item = "cc_%".format(i+1).asSymbol;}).do({arg paramName, i;
			var path = "/midi/%".format(paramName).asSymbol;
			Ndef(path, 0.0).kr(1);
			MIDIdef.cc("cc_%Responder".format(paramName).asSymbol, {arg val, num, chan, src;
				Ndef(path).bus.set(val);
			},
			ccNum: 1+i,
			chan: 0
			);
		});

		40.collect({arg i, item; item = "button_%".format(i+1).asSymbol;}).do({arg paramName, i;
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

	// bank A
	// rad one

	Ndef(\knirk, {arg bufnum, numChannels;
		var sig, mod, play, env, sum;
		var rate = Ndef.kr('/midi/cc_1').linlin(0,127,0.01,2.0);
		var trspeed = Ndef.kr('/midi/cc_6').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_11').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_16').linlin(0,127,0.0,3.0);
		var switch = Ndef.kr('/midi/button_6');

		var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger= Impulse.kr(1.4);
		bufnum = topEnvironment['filee'].bufnum;
		numChannels = topEnvironment['filee'].numChannels;

		play=PlayBufCF.ar(
			numChannels,
			bufnum,
			BufRateScale.kr(bufnum)*rate,
			Impulse.kr(trspeed),//trigger,
			startPos, loop:1, lag: 0.3) * 4;


		sig = play;
		sig = sig.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sum = CompanderD.ar(sig,0.5,1,1/3);
		sum = Limiter.ar(sum, 0.99, 0.00001);
		sum = sum.flat;
		LeakDC.ar(sum);
	}).playN([0,1]);

	// rad two
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
		sum=(sum*3*ficAmp.lag*switch).clip(-0.99,0.99);
		sum = Limiter.ar(sum, 0.99);
		sum = sum.flat;
		LeakDC.ar(sum);
	}).playN([0,1]);

	// rad 3 . 4
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
		sum=squi+LPF.ar(squi, 100, 2)* 0.5;
		sum = Pulse.ar(LFNoise0.kr(pulsechangespeed).abs.linlin(0.0,1.0,pulselo,pulsehi))*sum;
		sum=(sum*ficAmp.lag*switch).clip(-0.99,0.99);
		sum = CompanderD.ar(sum,0.5,1,1/3);
		sum = LeakDC.ar(sum);
		sum = Limiter.ar(sum, 0.99);
		sum = sum.flat;
		sum;
	}).playN([0,1]);

	// rad 5
	Ndef(\albo, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var cut = Ndef.kr('/midi/cc_5');
		var pdisp = Ndef.kr('/midi/cc_10').linlin(0,127,0.01,100.0).lag;
		var pos = Ndef.kr('/midi/cc_15').linlin(0,127,-1.0,1.0).lag;
		var amp = Ndef.kr('/midi/cc_20').linlin(0,127,0.0,4.0).lag;
		var switch = Ndef.kr('/midi/button_10');

		bufnum = topEnvironment['birds'].bufnum;
		numChannels = topEnvironment['birds'].numChannels;

		sig=PlayBuf.ar(
			numChannels,
			bufnum,
			BufRateScale.kr(bufnum)*cut.linexp(0,127,0.2,1).lag,
			switch, loop: 1);
		sig = LinXFade2.ar(sig, PitchShift.ar(sig, 1.0, 30.6, pdisp, 30, 1.3), pos);
		sig = Splay.ar(sig*2);
		sig = DFM1.ar(sig, cut.linexp(0,127,2000,22e3).lag);
		sig = CompanderD.ar(sig, 0.6, 1, 1/3);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);

	}).playN([0,1]);

	// bank B
	// rad 1
	Ndef(\frogys, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var rate = Ndef.kr('/midi/cc_21').linlin(0,127,0.01,2.0);
		var trspeed = Ndef.kr('/midi/cc_26').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_31').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_36').linlin(0,127,0.0,1.0);
		var switch = Ndef.kr('/midi/button_16');

		var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger= Impulse.kr(1.4);
		bufnum = topEnvironment['frogs'].bufnum;
		numChannels = topEnvironment['frogs'].numChannels;

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

		sig=(play1+play2+play3)*0.7;
		sig=sig.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		sig;
	}).playN([0,1]);

	//rad 2
	Ndef(\somert, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var rate = Ndef.kr('/midi/cc_22').linlin(0,127,0.01,2.0);
		var trspeed = Ndef.kr('/midi/cc_27').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_32').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_37').linlin(0,127,0.0,1.0);
		var switch = Ndef.kr('/midi/button_17');

		var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger= Impulse.kr(1.4);
		bufnum = topEnvironment['somer'].bufnum;
		numChannels = topEnvironment['somer'].numChannels;

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

		sig=(play1+play2+play3)*0.7;
		sig=sig.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		sig;
	}).playN([0,1]);

	// rad 3
	Ndef(\kbas, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var rate = Ndef.kr('/midi/cc_23').linlin(0,127,0.01,2.0);
		var trspeed = Ndef.kr('/midi/cc_28').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_33').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_38').linlin(0,127,0.0,1.0);
		var switch = Ndef.kr('/midi/button_18');

		var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger= Impulse.kr(1.4);
		bufnum = topEnvironment['klikbas'].bufnum;
		numChannels = topEnvironment['klikbas'].numChannels;

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

		sig=(play1+play2+play3)*0.7;
		sig=sig.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);

	// rad 4
	Ndef(\oits, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var rate = Ndef.kr('/midi/cc_24').linlin(0,127,0.01,2.0);
		var trspeed = Ndef.kr('/midi/cc_29').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_34').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_39').linlin(0,127,0.0,1.0);
		var switch = Ndef.kr('/midi/button_19');

		var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger= Impulse.kr(1.4);
		bufnum = topEnvironment['oita'].bufnum;
		numChannels = topEnvironment['oita'].numChannels;

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

		sig=(play1+play2+play3)*0.7;
		sig=sig.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);

	// rad 5
	Ndef(\snuppi, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var rate = Ndef.kr('/midi/cc_25').linlin(0,127,0.01,2.0);
		var trspeed = Ndef.kr('/midi/cc_30').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_35').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_40').linlin(0,127,0.0,1.0);
		var switch = Ndef.kr('/midi/button_20');

		var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger= Impulse.kr(1.4);
		bufnum = topEnvironment['snappa'].bufnum;
		numChannels = topEnvironment['snappa'].numChannels;

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

		sig=(play1+play2+play3)*0.7;
		sig=sig.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);

	// BANK C
	// rad 1
	Ndef(\sdrtrakki, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var cut = Ndef.kr('/midi/cc_41').linexp(0,127,20,22e3).lag;
		var res = Ndef.kr('/midi/cc_46').linlin(0,127,0.01,1.0).lag;
		var pos = Ndef.kr('/midi/cc_51').linlin(0,127,-1.0,1.0).lag;
		var amp = Ndef.kr('/midi/cc_56').linlin(0,127,0.0,1.0).lag;
		var switch = Ndef.kr('/midi/button_26');

		bufnum = topEnvironment['sdr_trak'].bufnum;
		numChannels = topEnvironment['sdr_trak'].numChannels;

		sig=PlayBuf.ar(
			numChannels,
			bufnum,
			BufRateScale.kr(bufnum),
			switch, loop: 1);
		sig = LinXFade2.ar(sig, PitchShift.ar(sig, 0.2, 1, 6), pos);
		sig = Splay.ar(sig);
		sig = DFM1.ar(sig, cut, res);
		sig = sig + (LPF.ar(sig, 120)*2.5);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);

	//rad 2
	Ndef(\sdrdroner, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		//var rate = Ndef.kr('/midi/cc_42').linlin(0,127,0.01,2.0);
		//var trspeed = Ndef.kr('/midi/cc_47').linlin(0,127,0.0,5.0);
		//var moth = Ndef.kr('/midi/cc_52').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_57').linlin(0,127,0.0,1.0);
		var switch = Ndef.kr('/midi/button_27');

		bufnum = topEnvironment['sdr_dron'].bufnum;
		numChannels = topEnvironment['sdr_dron'].numChannels;

		sig=PlayBuf.ar(
			numChannels,
			bufnum,
			BufRateScale.kr(bufnum),
			switch, loop: 1);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);

	// rad 3
	Ndef(\sdr_rytme, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var cut = Ndef.kr('/midi/cc_43').linexp(0,127,20,22e3).lag;
		var res = Ndef.kr('/midi/cc_48').linlin(0,127,0.01,1.0).lag;
		var pos = Ndef.kr('/midi/cc_53').linlin(0,127,-1.0,1.0).lag;
		var amp = Ndef.kr('/midi/cc_58').linlin(0,127,0.0,1.0);
		var switch = Ndef.kr('/midi/button_28');

		bufnum = topEnvironment['sdr_ry'].bufnum;
		numChannels = topEnvironment['sdr_ry'].numChannels;

		sig=PlayBuf.ar(
			numChannels,
			bufnum,
			BufRateScale.kr(bufnum)*0.8,
			switch, loop: 1);
		sig = LinXFade2.ar(sig, PitchShift.ar(sig, 0.8, 0.6, 6), pos);
		sig = Splay.ar(sig);
		sig = DFM1.ar(sig, cut, res);
		sig = sig + (LPF.ar(sig, 120)*1.5);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);

	// rad 4
	Ndef(\insekto, {arg bufnum, numChannels;
		var sig, mod, play1, play2, play3, env;
		var rate = Ndef.kr('/midi/cc_44').linexp(0,127,0.01,2.0).lag;
		var trspeed = Ndef.kr('/midi/cc_49').linlin(0,127,0.0,5.0);
		var moth = Ndef.kr('/midi/cc_54').linlin(0,127,0.0,1.0);
		var amp = Ndef.kr('/midi/cc_59').linlin(0,127,0.0,0.8);
		var switch = Ndef.kr('/midi/button_29');

		var startPos=Phasor.kr(1, 1, moth, 1, 0)*BufFrames.kr(bufnum);
		var trigger= Impulse.kr(1.4);
		bufnum = topEnvironment['fish'].bufnum;
		numChannels = topEnvironment['fish'].numChannels;

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

		sig=(play1+play2+play3)*0.7;
		sig = DFM1.ar(sig, rate.linexp(0.01,2.0,1000,17e3), 0.08);
		//sig=sig.clip(-0.99,0.99);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);



	// BANK D
	Ndef(\teko, {var sig;
		var st1 = Ndef.kr('/midi/cc_61').linlin(0,127,0.05,8.0);
		var st2 = Ndef.kr('/midi/cc_62').linlin(0,127,0.05,8.0);
		var st3 = Ndef.kr('/midi/cc_63').linlin(0,127,0.05,8.0);
		var st4 = Ndef.kr('/midi/cc_64').linlin(0,127,0.05,8.0);
		var st5 = Ndef.kr('/midi/cc_65').linlin(0,127,0.05,8.0);
		var dur = Ndef.kr('/midi/cc_71').linexp(0,127,0.005,5.0);
		var devi = Ndef.kr('/midi/cc_66').linexp(0,127,1,19.0);

		var amp = Ndef.kr('/midi/cc_76').linlin(0,127,0.0,1.5);
		var switch = Ndef.kr('/midi/button_36');

		var snd, sndamp;
		var freqskew = LFNoise1.ar(1.4).range(1, 4000);
		var freqinit = Ndef.kr('/midi/cc_71').linexp(0,127,0.01,6.0);
		var freq, freqenv, ampenv, freqenvctl, ampenvctl, t_gate;

		var seq = Duty.ar(1, 0, Dseq([st1,st2,st3,st4,st5], inf));

		t_gate = Impulse.kr(seq);
		freqenv = Env([0.8, 1, 0.3, 0], [0.15, 0.7, 0.25], 'sine');
		ampenv = Env([0, 1, 0.7, 1, 0], [0.005, 0.95, 0.15, 2.25], 'sine');
		freq = EnvGen.kr(freqenv, t_gate, freqskew, freqinit, dur);
		sndamp = EnvGen.kr(ampenv, t_gate, 0.8, 0, dur, 0);
		snd = Pluck.ar(CuspL.ar(
			((1..10)/devi)*freq,
			1,
			mul: sndamp
		), t_gate);
		snd = DFM1.ar(snd, sndamp.linexp(0,1,20,16e3).lag,);
		sig = Splay.ar(snd);
		sig = sig + (LPF.ar(sig, 60)*1.2);
		sig = sig*amp.lag;
		sig = sig * ToggleFF.ar(Trig.ar(switch)).lag;
		sig = sig.flat;
		LeakDC.ar(sig);
	}).playN([0,1]);


	//"done loading".postln;
};
)








