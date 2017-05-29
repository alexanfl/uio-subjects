function [Aincr] = Aincrement (Aref, db)
  Aincr = Aref*10^(db/10);
endfunction
