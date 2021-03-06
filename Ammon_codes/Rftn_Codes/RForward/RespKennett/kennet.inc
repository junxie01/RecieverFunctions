c
c	model parameters
c
      integer*2 mxlr,allrvb,onervb,norvb,allphs,prmphs,cnvphs
      parameter ( mxlr=45, allrvb=-1, onervb=1, norvb=0 )
      parameter ( allphs=0, prmphs=1, cnvphs=-1 )
      real qp(mxlr),qs(mxlr),rho(mxlr),thik(mxlr)
      complex alfa(mxlr),beta(mxlr)
      common /model/ alfa,beta,qp,qs,rho,thik
c
c	interface reflection and transmission coefficients
c
      complex rupp(mxlr),rups(mxlr),rusp(mxlr),russ(mxlr),rush(mxlr)
      complex rdpp(mxlr),rdps(mxlr),rdsp(mxlr),rdss(mxlr),rdsh(mxlr)
      complex tupp(mxlr),tups(mxlr),tusp(mxlr),tuss(mxlr),tush(mxlr)
      complex tdpp(mxlr),tdps(mxlr),tdsp(mxlr),tdss(mxlr),tdsh(mxlr)
      complex xi(mxlr),eta(mxlr)
      complex ruppfs,rupsfs,ruspfs,russfs,rushfs
      complex dvpfs,dvsfs,drpfs,drsfs,dtshfs
      integer*2 cnvrsn(0:mxlr), reverb(mxlr)
      common /rfltrn/ rupp,rups,rusp,russ,rush,
     1                rdpp,rdps,rdsp,rdss,rdsh,
     2                tupp,tups,tusp,tuss,tush,
     3                tdpp,tdps,tdsp,tdss,tdsh,
     4                ruppfs,rupsfs,ruspfs,russfs,rushfs,
     5                dvpfs,dvsfs,drpfs,drsfs,dtshfs,
     6                xi,eta,
     7                cnvrsn,reverb
c
c	interface and layer matricies for perturbations
c	used in partial derivative calculations for
c	inversion process
c
      complex mu11(mxlr),mu12(mxlr),mu21(mxlr),mu22(mxlr)
      complex md11(mxlr),md12(mxlr),md21(mxlr),md22(mxlr)
      complex nu11(mxlr),nu12(mxlr),nu21(mxlr),nu22(mxlr)
      complex nd11(mxlr),nd12(mxlr),nd21(mxlr),nd22(mxlr)
      complex mu(mxlr),epa(mxlr),epb(mxlr),zsh(mxlr)
      complex darupp(mxlr),darups(mxlr),darusp(mxlr),daruss(mxlr),
     1        darush(mxlr)
      complex dardpp(mxlr),dardps(mxlr),dardsp(mxlr),dardss(mxlr),
     1        dardsh(mxlr)
      complex datupp(mxlr),datups(mxlr),datusp(mxlr),datuss(mxlr),
     1        datush(mxlr)
      complex datdpp(mxlr),datdps(mxlr),datdsp(mxlr),datdss(mxlr),
     1        datdsh(mxlr)
      complex dbrupp(mxlr),dbrups(mxlr),dbrusp(mxlr),dbruss(mxlr),
     1        dbrush(mxlr)
      complex dbrdpp(mxlr),dbrdps(mxlr),dbrdsp(mxlr),dbrdss(mxlr),
     1        dbrdsh(mxlr)
      complex dbtupp(mxlr),dbtups(mxlr),dbtusp(mxlr),dbtuss(mxlr),
     1        dbtush(mxlr)
      complex dbtdpp(mxlr),dbtdps(mxlr),dbtdsp(mxlr),dbtdss(mxlr),
     1        dbtdsh(mxlr)
      complex dxi(mxlr),deta(mxlr)
      complex druppfs,drupsfs,druspfs,drussfs,drushfs
      complex ddvpfs,ddvsfs,ddrpfs,ddrsfs,ddtshfs
      complex srndpp(2:mxlr-1),srndps(2:mxlr-1),
     1        srndsp(2:mxlr-1),srndss(2:mxlr-1),
     2        srndsh(2:mxlr-1)
      complex stnupp(2:mxlr-1),stnups(2:mxlr-1),
     1        stnusp(2:mxlr-1),stnuss(2:mxlr-1),
     2        stnush(2:mxlr-1)
      complex srnupp(mxlr-2),srnups(mxlr-2),
     1        srnusp(mxlr-2),srnuss(mxlr-2),
     2        srnush(mxlr-2)
      complex sdvp(mxlr-2),sdvs(mxlr-2),
     1        sdrp(mxlr-2),sdrs(mxlr-2),
     2        sdts(mxlr-2)
      common /invsav1/ mu11,mu12,mu21,mu22,
     1                md11,md12,md21,md22,
     2                nu11,nu12,nu21,nu22,
     3                nd11,nd12,nd21,nd22,
     4                mu,epa,epb,zsh
      common /invsav2/ darupp,darups,darusp,daruss,darush,
     1                dardpp,dardps,dardsp,dardss,dardsh,
     2                datupp,datups,datusp,datuss,datush,
     3                datdpp,datdps,datdsp,datdss,datdsh,
     4                dbrupp,dbrups,dbrusp,dbruss,dbrush,
     5                dbrdpp,dbrdps,dbrdsp,dbrdss,dbrdsh,
     6                dbtupp,dbtups,dbtusp,dbtuss,dbtush,
     7                dbtdpp,dbtdps,dbtdsp,dbtdss,dbtdsh,
     8                dxi,deta
      common /invsav3/ druppfs,drupsfs,druspfs,drussfs,drushfs,
     1                ddvpfs,ddvsfs,ddrpfs,ddrsfs,ddtshfs
      common /invsav4/ srndpp,srndps,srndsp,srndss,srndsh,
     1                 stnupp,stnups,stnusp,stnuss,stnush,
     2                 srnupp,srnups,srnusp,srnuss,srnush,
     3                 sdvp,sdvs,sdrp,sdrs,sdts
c
c	source parameters for moment tensor sources
c
      integer*2 srclyr,nsrc
      parameter ( nsrc=5 )
      real rhos
      complex alfas,betas
      complex mus11,mus12,mus21,mus22,mussh
      complex nus11,nus12,nus21,nus22,nussh
      complex mds11,mds12,mds21,mds22,mdssh
      complex nds11,nds12,nds21,nds22,ndssh
      complex pup0(nsrc),pdn0(nsrc),svup0(nsrc),svdn0(nsrc),
     *        shup0(nsrc),shdn0(nsrc)
      complex pupp1(nsrc),pdnp1(nsrc),svupp1(nsrc),svdnp1(nsrc),
     *        shupp1(nsrc),shdnp1(nsrc)
      complex pupm1(nsrc),pdnm1(nsrc),svupm1(nsrc),svdnm1(nsrc),
     *        shupm1(nsrc),shdnm1(nsrc)
      complex pupp2(nsrc),pdnp2(nsrc),svupp2(nsrc),svdnp2(nsrc),
     *        shupp2(nsrc),shdnp2(nsrc)
      complex pupm2(nsrc),pdnm2(nsrc),svupm2(nsrc),svdnm2(nsrc),
     *        shupm2(nsrc),shdnm2(nsrc)
      real mxx(nsrc),mxy(nsrc),mxz(nsrc),myy(nsrc),myz(nsrc),mzz(nsrc)
      common /srctrm/ mus11,mus12,mus21,mus22,mussh,
     *                nus11,nus12,nus21,nus22,nussh,
     *                mds11,mds12,mds21,mds22,mdssh,
     *                nds11,nds12,nds21,nds22,ndssh,
     *                pup0,pdn0,svup0,svdn0,shup0,shdn0,
     *                pupp1,pdnp1,svupp1,svdnp1,shupp1,shdnp1,
     *                pupm1,pdnm1,svupm1,svdnm1,shupm1,shdnm1,
     *                pupp2,pdnp2,svupp2,svdnp2,shupp2,shdnp2,
     *                pupm2,pdnm2,svupm2,svdnm2,shupm2,shdnm2,
     *                mxx,mxy,mxz,myy,myz,mzz,
     *                rhos,alfas,betas,srclyr
c
c	layered medium response for buried source
c
      complex dvpup,dvsup,drpup,drsup,dtsup,
     *        dvpdn,dvsdn,drpdn,drsdn,dtsdn
      common /respns/ dvpup,dvsup,drpup,drsup,dtsup,
     *                dvpdn,dvsdn,drpdn,drsdn,dtsdn
