function [] = playSound (A,frequencies,curve,t,N,Fs,ch,plt)
  chL = 1;
  chR = 2
  n = length(frequencies);
  Aref = A
  I = zeros(n,1);

  for i = 1:n
    fin = 0;
    A = Aref;
    while fin == 0
      if A > 1
        A = 1;
      end
      A                                       % Print current A for each run.
      freq = frequencies(i)

      oscillation = sin(2*pi*freq*t);         % Sound-oscillation array.
      signal = A*oscillation.*curve;          % Modified sound array.
    
      stereosignal = zeros(N,2);      
      stereosignal(:,ch) = signal(1:N);       % Determine which channel to use.
      sound(stereosignal,Fs);             
      response = input("Press one of the following keys and then [Enter] \n...
        i: increase by 1 dB\n... 
        I: increase by 6 dB\n...
        d: decrease by 1 dB\n...
        D: done\n","s");
      if strcmp(response,'D')                 % Controls for audiometry test.
        fin = 1;
        I(i) = A;
      elseif strcmp(response,'d')
        A *= 0.891;
      elseif strcmp(response,'i')
        A *= 1.122;
      elseif strcmp(response,'I')
        A *= 1.995;
      else
        'Error: Try again!'
      end
    end
  end
  y = 10*log10(I/Aref);                       % Relative dB measure.

  if ch == chR & plt == 1                     % Plot audiogram.
    h = figure;
    semilogx(frequencies, y,  'LineWidth', 1.0,'o-');
    set(gca,'LineWidth', 1.0, 'FontSize', 21, 'FontName', 'times');
    xlabel('Frekvens [Hz]', 'FontSize', 21, 'FontName', 'times');
    ylabel('Svekkelse [dB]', 'FontSize', 21, 'FontName', 'times');
    grid on;
    print -color -depsc '../doc/rightear61.eps'
  elseif ch == chL & plt == 1
    h = figure;
    semilogx(frequencies, y, 'LineWidth', 1.0,'ro-');
    set(gca,'LineWidth', 1.0, 'FontSize', 21, 'FontName', 'times');
    xlabel('Frekvens [Hz]', 'FontSize', 21, 'FontName', 'times');
    ylabel('Svekkelse [dB]', 'FontSize', 21, 'FontName', 'times');
    grid on;
    print -color -depsc '../doc/leftear61.eps'
  end
endfunction
