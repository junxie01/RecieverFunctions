c      write a sac file data
       subroutine wdsac(file1,x)
       dimension x(1)
       character*32 file1
       include 'hdr.inc'
       inunit1=8
       k=(158+iheader(10))*4
       open(inunit1,file=file1,access='direct',recl=k)
       write(inunit1,rec=1) (rheader(i),i=1,70),(iheader(i),i=1,35)
     *  ,(lheader(i),i=1,5),kstnm,kevnm,(cheader(i),i=1,21)
     *  ,(x(i),i=1,iheader(10))
       close(inunit1)
       return
       end
