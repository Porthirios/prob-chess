drop procedure if exists statvar;
drop procedure if exists tstatvar;
drop procedure if exists seekopening;
delimiter //
create procedure statvar(v varchar(255))
language sql
sql security invoker
begin
  declare len,c int;
  set len=length(v);
  select count(*) from `cache` where variant=v into c;
  if c=0 then
    create table resvar (move varchar(10),cnt int,mwelo int, mbelo int, key move(move)) select mid(content,len+1,instr(mid(content,len+1),' ')) as move,result,count(*) as cnt, max(whiteelo) as mwelo, max(blackelo) as mbelo from pgn where left(content,len)=v group by move,result;
    insert into `cache` select v as variant, move, sum(wcnt)+sum(dcnt)+sum(lcnt) as cnt, sum(wcnt)/(sum(wcnt)+sum(dcnt)+sum(lcnt)) as pwin, sum(lcnt)/(sum(wcnt)+sum(dcnt)+sum(lcnt)) as plose, max(mwelo) as maxwelo, max(mbelo) as maxbelo from ( select move, w.cnt as wcnt, 0 as dcnt, 0 as lcnt,mwelo,mbelo from resvar as w where w.result='1-0' union select move, 0 as wcnt, d.cnt as dcnt, 0 as lcnt,mwelo,mbelo from resvar as d where d.result='1/2-1/2' union select move, 0 as wcnt, 0 as dcnt, l.cnt as lcnt,mwelo,mbelo from resvar as l where l.result='0-1') as rr group by move;
    drop table resvar;
  end if;
  select move,cnt,pwin,plose,maxwelo,maxbelo from `cache` where variant=v;
end //

create procedure tstatvar(v varchar(255))
language sql
sql security invoker
begin
  declare len int;
  set len=length(v);
  create table resvar (move varchar(10),cnt int,mwelo int, mbelo int, key move(move)) select mid(content,len+1,instr(mid(content,len+1),' ')) as move,result,count(*) as cnt, max(whiteelo) as mwelo, max(blackelo) as mbelo from twic where left(content,len)=v group by move,result;
  select move,w.cnt as wins,d.cnt as draws, l.cnt as loses, w.mwelo as maxwelo, l.mbelo as maxbelo from resvar as w left join resvar as d using(move) right join resvar as l using(move) where w.result='1-0' and d.result='1/2-1/2' and l.result='0-1';
  drop table resvar;
end //

create procedure seekopening(v varchar(255), minfrac real)
language sql
sql security invoker
begin
  declare len,total int;
  set len=length(v);
  select sum(cnt) from openings where variant=v group by variant into total;
  if isnull(total) then
    replace into openings (variant,opening,cnt) select v as variant, opening, count(*) as cnt from twic where left(content,len)=v group by opening;
    select sum(cnt) from openings where variant=v group by variant into total;
  end if;
  select opening,cnt/total as frac from openings where variant=v and cnt>=total*minfrac order by frac desc;
end //
