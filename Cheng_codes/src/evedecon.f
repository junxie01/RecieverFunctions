      program pwaveqn
c
c  ********************************************************************
c
c    fortran 77 program to perform a source equalization deconvolution
c     on a three component seismogram - using the method of langston (1979)
c
c    *** written by t.j. owens - july 1982
c    *** modified by G Randall for gaussian normalization
c    *** modified by C Ammon for water-level normaliztion 880711
c
c  *************************************************************************
c
      dimension d2(4000),caz(3)
      character eqfile*80,outfil*32,comp(3,2)*2,outc(2)*5,knm(2)*8
      character evnam*32,stanam*32
      complex data(4000,3)
      double precision gnorm
      logical yes,yesno,rel,rel1
      integer blank,ounit
c **********************************************************************
c
c common block info for link with subroutine sacio
c
c
c **************************************************************************
c
c   parameter definitions may be found in sacio comments
c
      common /win/ data
      common /innout/ inunit,ounit
      data
     *     comp/'.z','.n','.e','.z','.r','.t'/
     *    ,outc/'.eqr ','.eqt '/
     *     ,knm/'radial  ','tangentl'/
      outfil='                                '
      inunit=5
      ounit=6
      pi=3.141592654
  2   call asktxt('Specify event name: ',evnam)
      call asktxt('specify station name: ',stanam)
      rel=yesno('Geographic(zne) data (y or n)? ')
      b1=ask('p wave ending time(in Sec): ')
      iblank1=blank(evnam)
      iblank2=blank(stanam)
      iblank0=22+iblank2+1
      iblank=iblank0+iblank1+iblank2
      eqfile(1:iblank0)='/var/data/field/pwave/'//
     *   stanam(1:iblank2)//'/'
      isyntp=2
      if(rel) isyntp=1
      do 1 i=1,3
         call zero(data(1,i),1,8000)
         eqfile(1:iblank + 3)=eqfile(1:iblank0)//evnam(1:iblank1)
     *   //'/'//stanam(1:iblank2)//comp(i,isyntp)
         call rsac1(eqfile,data(1,i),npts,beg,dt,8000,nerr)
         call getfhv('cmpaz',caz(i),nerr)
    1 continue
      call getfhv('a',a0,nerr)
      call getfhv('baz',baz,nerr)
       k12=(b1+a0-beg)/dt+1
       k11=1
      do 3 i=1,3
      call getdata(data(1,i),k11,k12,npts)
    3 continue
      npts=k12-k11+1
       if(rel) call rotate(data,8000,3,baz,caz,npts)
c
c    *******************************************************************
c
      yes=yesno('Window data (y or n)? ')
      if(.not.yes) go to 4
         blen=ask('Length of window (in secs): ')
         call window(blen,npts,dt,3)
    4 nft=npowr2(npts)
      nfpts=nft/2 + 1
      fny=1./(2.*dt)
      delf=fny/float(nft/2)
      write(ounit,102) npts,nft,fny,delf,dt
  102 format(1x,'npts=',i5,1x,'nft=',i5,1x,'fny=',f7.4,1x,
     *          'delf=',f8.4,1x,'dt=',f6.3)
c
c  change delf to normalize deconvolution so dfftr works
c  dt from forward step is cancelled in decon, so no delf
c  on inverse, just 1/nft
c
      cdelf = 1. / float( nft )
c      call asktxt('Specify outfil directory: ',outfil)
c      iblank=blank(outfil)
      do 5 i=1,3
         call dfftr(data(1,i),nft,'forward',dt)
         if(i.ne.1) go to 5
         d2max=0.
         do 7 j=1,nfpts
            d2(j)=real(data(j,i)*conjg(data(j,i)))
            if(d2(j).gt.d2max) d2max=d2(j)
    7    continue
    5 continue
      decon=1.
      c=ask('Trough filler, c =  ')
      agauss=ask('Gaussian scale, a = ')
c      tdelay=ask('Enter phase shift: ')
c      tdelay=a0-beg
       tdelay=b2
      t0=tdelay
      phi1=c*d2max
      gnorm = 0.0d0
      do 8 i=1,2
         do 9 j=1,nfpts
            freq=float(j-1)*delf
            w=2.*pi*freq
            phi=phi1
            if(d2(j).gt.phi) phi=d2(j)
            gauss=-w*w/(4.*agauss*agauss)
	    if(i.eq.1) gnorm = gnorm + exp( gauss )
            data(j,i+1)=data(j,i+1)*conjg(data(j,1))*
     *                   cmplx(exp(gauss)/phi,0.)
            data(j,i+1)=data(j,i+1)*exp(cmplx(0.,-w*tdelay))
    9    continue
         call dfftr(data(1,i+1),nft,'inverse',cdelf)
    8 continue
c
c     deconvolve the vertical from itself using the
c         specified water-level parameter and gaussian
c
      do 19 j=1,nfpts
	 freq=float(j-1)*delf
	 w=2.*pi*freq
	 phi=phi1
	 gauss=-w*w/(4.*agauss*agauss)
	 if(d2(j).gt.phi) phi=d2(j)
	 data(j,1)=data(j,1)*conjg(data(j,1))*
     *                   cmplx(exp(gauss)/phi,0.)
	 data(j,1)=data(j,1)*exp(cmplx(0.,-w*tdelay))
19    continue

c
c*************************************************************
c
c     inverse transform the equalized vertical component
c
      call dfftr(data(1,1),nft,'inverse',cdelf)

c
c     compute the maximum value of the vertical component
c        to include it in the normalization factor gnorm
c
      call minmax(data(1,1),npts,dmin,dmax,dmean)
c
c     normalize the dmax for the transforms and gaussian
c
c     dmax = dmax *  float(nfpts) / gnorm
c
c************************************************************* 
c
c     gnorm = dmax * gnorm / nfpts
c
c     note that (dmax * gnorm / nfpts) = the unormalized dmax
c
      gnorm = dmax
      do 111 i = 2,3
      do 111 j = 1,npts
	 data(j,i) = data(j,i) / gnorm
 111  continue
      call newhdr
      beg=-1*t0
      e=beg+dt*(npts-1)
      call setfhv('b',beg,nerr)
      call setfhv('e',e,nerr)
      call setfhv('delta',dt,nerr)
      call setnhv('npts',npts,nerr)
      call setfhv('user0',agauss,nerr)
      call setfhv('user1',t0,nerr)
      do 11 i=2,3
           outfil(1:iblank1 + 5)=evnam(1:iblank1)//outc(i-1)
           if(i.eq.2) caz(i)=baz+180
           if(i.eq.3) caz(i)=baz+270
           if(caz(i).gt.360.) caz(i)=caz(i)-360.
           call minmax(data(1,i),npts,dmin,dmax,dmean)
           call setfhv('cmpaz',caz(i),nerr)
           call setfhv('cmpinc',90.,nerr)
           call setfhv('depmin',dmin,nerr)
           call setfhv('depmax',dmax,nerr)
           call setfhv('depmen',dmean,nerr)
           call wsac0(outfil,xxx,data(1,i),nerr)
   11 continue
      yes=yesno('Try another (y or n)? ')
      if(yes) go to 2
      stop
      end
      subroutine window(b0,npts,dt,ndat)
      dimension data(8000,3)
      common /win/ data
      data pi/3.1415926/
      bb=pi/b0
      nend=ifix((b/dt)+.5) +1
      do 1 i=1,nend
      t=float(i-1)*dt
      windo=.5*(1. + cos(bb*t+pi))
         do 2 j=1,ndat
         data(i,j)=data(i,j)*windo
         data(npts+1-i,j)=data(npts+1-i,j)*windo
    2    continue
    1 continue
      return
      end
