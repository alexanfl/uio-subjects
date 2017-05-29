Fs = 44100;                                         % Sampling frequency
frequencies = [60,120,250,500,1000,2000,4000,8000]; % [Hertz]
T = 1;                                              % Play time.
N = Fs*T;                                           % Number of elements.
t = linspace(0,T,N);
tCut = 0.1;                                         % For fading in/out.

ch = input("Which ear would you like to test? Press L or R then [Enter].\n","s");
if strcmp(ch,'L')                                   % Test to choose channel.
  ch = 1;
elseif strcmp(ch, 'R')
  ch = 2;
else
  Channel = 'Invalid. Exiting.'
  break;
end

plt = input("Would you like to plot the results? Press y or n then [Enter].\n","s");
if strcmp(plt, 'n')                                 % Plotting audiogram.
  plt = 0;
elseif strcmp(plt, 'y')
  plt = 1;
else
  Plot = 'Invalid. Exiting.'
  break;
end

curve = createSigmoid(t,tCut,'alg',N,T,1);          % "Omhyllingskurve" for fade.
A = 0.025;                                          % Referance amplitude.
playSound(A,frequencies,curve,t,N,Fs,ch,plt);   
