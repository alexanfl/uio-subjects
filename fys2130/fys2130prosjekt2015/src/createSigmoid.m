function [curve] = createSigmoid (t,tCut,funcType,N,T,plt)
  tLow = tCut*(N/T)                 % Stop fading in.
  tHigh = N-tCut*(N/T)+1            % Start fading out.

  top = sigmoid(funcType,tCut)      % Amplitude; determines the sound level. 
  
  t1 = t(1,1:tLow);
  t2 = t(1,tHigh:N);

  curve = top*ones(1,N);
  curve(1,1:tLow) = sigmoid(funcType,t1);
  curve(1,tHigh:N) = sigmoid(funcType,fliplr(t2)-(T-tCut));
  if plt == 1
    plot(t,curve);
    set(gca,'LineWidth', 1.0, 'FontSize', 21, 'FontName', 'times');
    xlabel('Tid [s]', 'FontSize', 21, 'FontName', 'times');
    ylabel('Amplitude', 'FontSize', 21, 'FontName', 'times');
    grid on;
    print -color -depsc '../doc/omhyll.eps'
  end
endfunction
