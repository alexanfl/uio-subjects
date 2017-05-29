function [curve] = sigmoid(str,t)
% This function returns a sigmoid-curve array 
% for a given array t and a keyword str.

if strcmp(str, 'arc')
  output = 'Used arctan sigmoid'
  curve = (2/pi)*atan((pi/2)*t);
elseif strcmp(str, 'alg')
  output = 'Used algebraic sigmoid'
  curve = 10.049847242*t./sqrt(1+t.^2);
elseif strcmp(str, 'alg3')
  output = 'Used algebraic sigmoid'
  curve = t./sqrt(1+t.^2);
elseif strcmp(str, 'alg2')
  output = 'Used algebraic 2 sigmoid'
  curve = t./(1+abs(t));
else strcmp(str, 'tanh')
  output = 'Used tanh sigmoid'
  curve = tanh(t);
end
endfunction
