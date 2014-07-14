/* ./src_f77/dggesx.f -- translated by f2c (version 20030320).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include <punc/vf2c.h>

/* Table of constant values */

static integer c__1 = 1;
static integer c__0 = 0;
static integer c_n1 = -1;
static doublereal c_b37 = 0.;
static doublereal c_b38 = 1.;

/* Subroutine */ int dggesx_(char *jobvsl, char *jobvsr, char *sort, L_fp 
	delctg, char *sense, integer *n, doublereal *a, integer *lda, 
	doublereal *b, integer *ldb, integer *sdim, doublereal *alphar, 
	doublereal *alphai, doublereal *beta, doublereal *vsl, integer *ldvsl,
	 doublereal *vsr, integer *ldvsr, doublereal *rconde, doublereal *
	rcondv, doublereal *work, integer *lwork, integer *iwork, integer *
	liwork, logical *bwork, integer *info, ftnlen jobvsl_len, ftnlen 
	jobvsr_len, ftnlen sort_len, ftnlen sense_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, b_dim1, b_offset, vsl_dim1, vsl_offset, 
	    vsr_dim1, vsr_offset, i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, ip;
    static doublereal pl, pr, dif[2];
    static integer ihi, ilo;
    static doublereal eps;
    static integer ijob;
    static doublereal anrm, bnrm;
    static integer ierr, itau, iwrk;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);
    static integer ileft, icols;
    static logical cursl, ilvsl, ilvsr;
    static integer irows;
    extern /* Subroutine */ int dlabad_(doublereal *, doublereal *), dggbak_(
	    char *, char *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, integer *, doublereal *, integer *, integer *, 
	    ftnlen, ftnlen), dggbal_(char *, integer *, doublereal *, integer 
	    *, doublereal *, integer *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, integer *, ftnlen);
    static logical lst2sl;
    extern doublereal dlamch_(char *, ftnlen), dlange_(char *, integer *, 
	    integer *, doublereal *, integer *, doublereal *, ftnlen);
    extern /* Subroutine */ int dgghrd_(char *, char *, integer *, integer *, 
	    integer *, doublereal *, integer *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *, integer *, integer *, 
	    ftnlen, ftnlen), dlascl_(char *, integer *, integer *, doublereal 
	    *, doublereal *, integer *, integer *, doublereal *, integer *, 
	    integer *, ftnlen);
    static logical ilascl, ilbscl;
    extern /* Subroutine */ int dgeqrf_(integer *, integer *, doublereal *, 
	    integer *, doublereal *, doublereal *, integer *, integer *), 
	    dlacpy_(char *, integer *, integer *, doublereal *, integer *, 
	    doublereal *, integer *, ftnlen);
    static doublereal safmin;
    extern /* Subroutine */ int dlaset_(char *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, integer *, ftnlen);
    static doublereal safmax;
    extern /* Subroutine */ int xerbla_(char *, integer *, ftnlen);
    static doublereal bignum;
    extern /* Subroutine */ int dhgeqz_(char *, char *, char *, integer *, 
	    integer *, integer *, doublereal *, integer *, doublereal *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     integer *, doublereal *, integer *, doublereal *, integer *, 
	    integer *, ftnlen, ftnlen, ftnlen), dtgsen_(integer *, logical *, 
	    logical *, logical *, integer *, doublereal *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, integer *, doublereal *, integer *, integer *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, integer *, integer *);
    static integer ijobvl, iright;
    extern integer ilaenv_(integer *, char *, char *, integer *, integer *, 
	    integer *, integer *, ftnlen, ftnlen);
    static integer ijobvr;
    static logical wantsb;
    static integer liwmin;
    static logical wantse, lastsl;
    static doublereal anrmto, bnrmto;
    extern /* Subroutine */ int dorgqr_(integer *, integer *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, integer *, 
	    integer *);
    static integer minwrk, maxwrk;
    static logical wantsn;
    static doublereal smlnum;
    extern /* Subroutine */ int dormqr_(char *, char *, integer *, integer *, 
	    integer *, doublereal *, integer *, doublereal *, doublereal *, 
	    integer *, doublereal *, integer *, integer *, ftnlen, ftnlen);
    static logical wantst, wantsv;


/*  -- LAPACK driver routine (version 3.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     June 30, 1999 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Function Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  DGGESX computes for a pair of N-by-N real nonsymmetric matrices */
/*  (A,B), the generalized eigenvalues, the real Schur form (S,T), and, */
/*  optionally, the left and/or right matrices of Schur vectors (VSL and */
/*  VSR).  This gives the generalized Schur factorization */

/*       (A,B) = ( (VSL) S (VSR)**T, (VSL) T (VSR)**T ) */

/*  Optionally, it also orders the eigenvalues so that a selected cluster */
/*  of eigenvalues appears in the leading diagonal blocks of the upper */
/*  quasi-triangular matrix S and the upper triangular matrix T; computes */
/*  a reciprocal condition number for the average of the selected */
/*  eigenvalues (RCONDE); and computes a reciprocal condition number for */
/*  the right and left deflating subspaces corresponding to the selected */
/*  eigenvalues (RCONDV). The leading columns of VSL and VSR then form */
/*  an orthonormal basis for the corresponding left and right eigenspaces */
/*  (deflating subspaces). */

/*  A generalized eigenvalue for a pair of matrices (A,B) is a scalar w */
/*  or a ratio alpha/beta = w, such that  A - w*B is singular.  It is */
/*  usually represented as the pair (alpha,beta), as there is a */
/*  reasonable interpretation for beta=0 or for both being zero. */

/*  A pair of matrices (S,T) is in generalized real Schur form if T is */
/*  upper triangular with non-negative diagonal and S is block upper */
/*  triangular with 1-by-1 and 2-by-2 blocks.  1-by-1 blocks correspond */
/*  to real generalized eigenvalues, while 2-by-2 blocks of S will be */
/*  "standardized" by making the corresponding elements of T have the */
/*  form: */
/*          [  a  0  ] */
/*          [  0  b  ] */

/*  and the pair of corresponding 2-by-2 blocks in S and T will have a */
/*  complex conjugate pair of generalized eigenvalues. */


/*  Arguments */
/*  ========= */

/*  JOBVSL  (input) CHARACTER*1 */
/*          = 'N':  do not compute the left Schur vectors; */
/*          = 'V':  compute the left Schur vectors. */

/*  JOBVSR  (input) CHARACTER*1 */
/*          = 'N':  do not compute the right Schur vectors; */
/*          = 'V':  compute the right Schur vectors. */

/*  SORT    (input) CHARACTER*1 */
/*          Specifies whether or not to order the eigenvalues on the */
/*          diagonal of the generalized Schur form. */
/*          = 'N':  Eigenvalues are not ordered; */
/*          = 'S':  Eigenvalues are ordered (see DELZTG). */

/*  DELZTG  (input) LOGICAL FUNCTION of three DOUBLE PRECISION arguments */
/*          DELZTG must be declared EXTERNAL in the calling subroutine. */
/*          If SORT = 'N', DELZTG is not referenced. */
/*          If SORT = 'S', DELZTG is used to select eigenvalues to sort */
/*          to the top left of the Schur form. */
/*          An eigenvalue (ALPHAR(j)+ALPHAI(j))/BETA(j) is selected if */
/*          DELZTG(ALPHAR(j),ALPHAI(j),BETA(j)) is true; i.e. if either */
/*          one of a complex conjugate pair of eigenvalues is selected, */
/*          then both complex eigenvalues are selected. */
/*          Note that a selected complex eigenvalue may no longer satisfy */
/*          DELZTG(ALPHAR(j),ALPHAI(j),BETA(j)) = .TRUE. after ordering, */
/*          since ordering may change the value of complex eigenvalues */
/*          (especially if the eigenvalue is ill-conditioned), in this */
/*          case INFO is set to N+3. */

/*  SENSE   (input) CHARACTER */
/*          Determines which reciprocal condition numbers are computed. */
/*          = 'N' : None are computed; */
/*          = 'E' : Computed for average of selected eigenvalues only; */
/*          = 'V' : Computed for selected deflating subspaces only; */
/*          = 'B' : Computed for both. */
/*          If SENSE = 'E', 'V', or 'B', SORT must equal 'S'. */

/*  N       (input) INTEGER */
/*          The order of the matrices A, B, VSL, and VSR.  N >= 0. */

/*  A       (input/output) DOUBLE PRECISION array, dimension (LDA, N) */
/*          On entry, the first of the pair of matrices. */
/*          On exit, A has been overwritten by its generalized Schur */
/*          form S. */

/*  LDA     (input) INTEGER */
/*          The leading dimension of A.  LDA >= max(1,N). */

/*  B       (input/output) DOUBLE PRECISION array, dimension (LDB, N) */
/*          On entry, the second of the pair of matrices. */
/*          On exit, B has been overwritten by its generalized Schur */
/*          form T. */

/*  LDB     (input) INTEGER */
/*          The leading dimension of B.  LDB >= max(1,N). */

/*  SDIM    (output) INTEGER */
/*          If SORT = 'N', SDIM = 0. */
/*          If SORT = 'S', SDIM = number of eigenvalues (after sorting) */
/*          for which DELZTG is true.  (Complex conjugate pairs for which */
/*          DELZTG is true for either eigenvalue count as 2.) */

/*  ALPHAR  (output) DOUBLE PRECISION array, dimension (N) */
/*  ALPHAI  (output) DOUBLE PRECISION array, dimension (N) */
/*  BETA    (output) DOUBLE PRECISION array, dimension (N) */
/*          On exit, (ALPHAR(j) + ALPHAI(j)*i)/BETA(j), j=1,...,N, will */
/*          be the generalized eigenvalues.  ALPHAR(j) + ALPHAI(j)*i */
/*          and BETA(j),j=1,...,N  are the diagonals of the complex Schur */
/*          form (S,T) that would result if the 2-by-2 diagonal blocks of */
/*          the real Schur form of (A,B) were further reduced to */
/*          triangular form using 2-by-2 complex unitary transformations. */
/*          If ALPHAI(j) is zero, then the j-th eigenvalue is real; if */
/*          positive, then the j-th and (j+1)-st eigenvalues are a */
/*          complex conjugate pair, with ALPHAI(j+1) negative. */

/*          Note: the quotients ALPHAR(j)/BETA(j) and ALPHAI(j)/BETA(j) */
/*          may easily over- or underflow, and BETA(j) may even be zero. */
/*          Thus, the user should avoid naively computing the ratio. */
/*          However, ALPHAR and ALPHAI will be always less than and */
/*          usually comparable with norm(A) in magnitude, and BETA always */
/*          less than and usually comparable with norm(B). */

/*  VSL     (output) DOUBLE PRECISION array, dimension (LDVSL,N) */
/*          If JOBVSL = 'V', VSL will contain the left Schur vectors. */
/*          Not referenced if JOBVSL = 'N'. */

/*  LDVSL   (input) INTEGER */
/*          The leading dimension of the matrix VSL. LDVSL >=1, and */
/*          if JOBVSL = 'V', LDVSL >= N. */

/*  VSR     (output) DOUBLE PRECISION array, dimension (LDVSR,N) */
/*          If JOBVSR = 'V', VSR will contain the right Schur vectors. */
/*          Not referenced if JOBVSR = 'N'. */

/*  LDVSR   (input) INTEGER */
/*          The leading dimension of the matrix VSR. LDVSR >= 1, and */
/*          if JOBVSR = 'V', LDVSR >= N. */

/*  RCONDE  (output) DOUBLE PRECISION array, dimension ( 2 ) */
/*          If SENSE = 'E' or 'B', RCONDE(1) and RCONDE(2) contain the */
/*          reciprocal condition numbers for the average of the selected */
/*          eigenvalues. */
/*          Not referenced if SENSE = 'N' or 'V'. */

/*  RCONDV  (output) DOUBLE PRECISION array, dimension ( 2 ) */
/*          If SENSE = 'V' or 'B', RCONDV(1) and RCONDV(2) contain the */
/*          reciprocal condition numbers for the selected deflating */
/*          subspaces. */
/*          Not referenced if SENSE = 'N' or 'E'. */

/*  WORK    (workspace/output) DOUBLE PRECISION array, dimension (LWORK) */
/*          On exit, if INFO = 0, WORK(1) returns the optimal LWORK. */

/*  LWORK   (input) INTEGER */
/*          The dimension of the array WORK.  LWORK >= 8*(N+1)+16. */
/*          If SENSE = 'E', 'V', or 'B', */
/*          LWORK >= MAX( 8*(N+1)+16, 2*SDIM*(N-SDIM) ). */

/*  IWORK   (workspace) INTEGER array, dimension (LIWORK) */
/*          Not referenced if SENSE = 'N'. */

/*  LIWORK  (input) INTEGER */
/*          The dimension of the array WORK.  LIWORK >= N+6. */

/*  BWORK   (workspace) LOGICAL array, dimension (N) */
/*          Not referenced if SORT = 'N'. */

/*  INFO    (output) INTEGER */
/*          = 0:  successful exit */
/*          < 0:  if INFO = -i, the i-th argument had an illegal value. */
/*          = 1,...,N: */
/*                The QZ iteration failed.  (A,B) are not in Schur */
/*                form, but ALPHAR(j), ALPHAI(j), and BETA(j) should */
/*                be correct for j=INFO+1,...,N. */
/*          > N:  =N+1: other than QZ iteration failed in DHGEQZ */
/*                =N+2: after reordering, roundoff changed values of */
/*                      some complex eigenvalues so that leading */
/*                      eigenvalues in the Generalized Schur form no */
/*                      longer satisfy DELZTG=.TRUE.  This could also */
/*                      be caused due to scaling. */
/*                =N+3: reordering failed in DTGSEN. */

/*  Further details */
/*  =============== */

/*  An approximate (asymptotic) bound on the average absolute error of */
/*  the selected eigenvalues is */

/*       EPS * norm((A, B)) / RCONDE( 1 ). */

/*  An approximate (asymptotic) bound on the maximum angular error in */
/*  the computed deflating subspaces is */

/*       EPS * norm((A, B)) / RCONDV( 2 ). */

/*  See LAPACK User's Guide, section 4.11 for more information. */

/*  ===================================================================== */

/*     .. Parameters .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Local Arrays .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

/*     Decode the input arguments */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    b_dim1 = *ldb;
    b_offset = 1 + b_dim1;
    b -= b_offset;
    --alphar;
    --alphai;
    --beta;
    vsl_dim1 = *ldvsl;
    vsl_offset = 1 + vsl_dim1;
    vsl -= vsl_offset;
    vsr_dim1 = *ldvsr;
    vsr_offset = 1 + vsr_dim1;
    vsr -= vsr_offset;
    --rconde;
    --rcondv;
    --work;
    --iwork;
    --bwork;

    /* Function Body */
    if (lsame_(jobvsl, "N", (ftnlen)1, (ftnlen)1)) {
	ijobvl = 1;
	ilvsl = FALSE_;
    } else if (lsame_(jobvsl, "V", (ftnlen)1, (ftnlen)1)) {
	ijobvl = 2;
	ilvsl = TRUE_;
    } else {
	ijobvl = -1;
	ilvsl = FALSE_;
    }

    if (lsame_(jobvsr, "N", (ftnlen)1, (ftnlen)1)) {
	ijobvr = 1;
	ilvsr = FALSE_;
    } else if (lsame_(jobvsr, "V", (ftnlen)1, (ftnlen)1)) {
	ijobvr = 2;
	ilvsr = TRUE_;
    } else {
	ijobvr = -1;
	ilvsr = FALSE_;
    }

    wantst = lsame_(sort, "S", (ftnlen)1, (ftnlen)1);
    wantsn = lsame_(sense, "N", (ftnlen)1, (ftnlen)1);
    wantse = lsame_(sense, "E", (ftnlen)1, (ftnlen)1);
    wantsv = lsame_(sense, "V", (ftnlen)1, (ftnlen)1);
    wantsb = lsame_(sense, "B", (ftnlen)1, (ftnlen)1);
    if (wantsn) {
	ijob = 0;
	iwork[1] = 1;
    } else if (wantse) {
	ijob = 1;
    } else if (wantsv) {
	ijob = 2;
    } else if (wantsb) {
	ijob = 4;
    }

/*     Test the input arguments */

    *info = 0;
    if (ijobvl <= 0) {
	*info = -1;
    } else if (ijobvr <= 0) {
	*info = -2;
    } else if (! wantst && ! lsame_(sort, "N", (ftnlen)1, (ftnlen)1)) {
	*info = -3;
    } else if (! (wantsn || wantse || wantsv || wantsb) || ! wantst && ! 
	    wantsn) {
	*info = -5;
    } else if (*n < 0) {
	*info = -6;
    } else if (*lda < max(1,*n)) {
	*info = -8;
    } else if (*ldb < max(1,*n)) {
	*info = -10;
    } else if (*ldvsl < 1 || ilvsl && *ldvsl < *n) {
	*info = -16;
    } else if (*ldvsr < 1 || ilvsr && *ldvsr < *n) {
	*info = -18;
    }

/*     Compute workspace */
/*      (Note: Comments in the code beginning "Workspace:" describe the */
/*       minimal amount of workspace needed at that point in the code, */
/*       as well as the preferred amount for good performance. */
/*       NB refers to the optimal block size for the immediately */
/*       following subroutine, as returned by ILAENV.) */

    minwrk = 1;
    if (*info == 0 && *lwork >= 1) {
	minwrk = (*n + 1 << 3) + 16;
	maxwrk = (*n + 1) * 7 + *n * ilaenv_(&c__1, "DGEQRF", " ", n, &c__1, 
		n, &c__0, (ftnlen)6, (ftnlen)1) + 16;
	if (ilvsl) {
/* Computing MAX */
	    i__1 = maxwrk, i__2 = (*n + 1 << 3) + *n * ilaenv_(&c__1, "DORGQR"
		    , " ", n, &c__1, n, &c_n1, (ftnlen)6, (ftnlen)1) + 16;
	    maxwrk = max(i__1,i__2);
	}
	work[1] = (doublereal) maxwrk;
    }
    if (! wantsn) {
	liwmin = 1;
    } else {
	liwmin = *n + 6;
    }
    iwork[1] = liwmin;

    if (*info == 0 && *lwork < minwrk) {
	*info = -22;
    } else if (*info == 0 && ijob >= 1) {
	if (*liwork < liwmin) {
	    *info = -24;
	}
    }

    if (*info != 0) {
	i__1 = -(*info);
	xerbla_("DGGESX", &i__1, (ftnlen)6);
	return 0;
    }

/*     Quick return if possible */

    if (*n == 0) {
	*sdim = 0;
	return 0;
    }

/*     Get machine constants */

    eps = dlamch_("P", (ftnlen)1);
    safmin = dlamch_("S", (ftnlen)1);
    safmax = 1. / safmin;
    dlabad_(&safmin, &safmax);
    smlnum = sqrt(safmin) / eps;
    bignum = 1. / smlnum;

/*     Scale A if max element outside range [SMLNUM,BIGNUM] */

    anrm = dlange_("M", n, n, &a[a_offset], lda, &work[1], (ftnlen)1);
    ilascl = FALSE_;
    if (anrm > 0. && anrm < smlnum) {
	anrmto = smlnum;
	ilascl = TRUE_;
    } else if (anrm > bignum) {
	anrmto = bignum;
	ilascl = TRUE_;
    }
    if (ilascl) {
	dlascl_("G", &c__0, &c__0, &anrm, &anrmto, n, n, &a[a_offset], lda, &
		ierr, (ftnlen)1);
    }

/*     Scale B if max element outside range [SMLNUM,BIGNUM] */

    bnrm = dlange_("M", n, n, &b[b_offset], ldb, &work[1], (ftnlen)1);
    ilbscl = FALSE_;
    if (bnrm > 0. && bnrm < smlnum) {
	bnrmto = smlnum;
	ilbscl = TRUE_;
    } else if (bnrm > bignum) {
	bnrmto = bignum;
	ilbscl = TRUE_;
    }
    if (ilbscl) {
	dlascl_("G", &c__0, &c__0, &bnrm, &bnrmto, n, n, &b[b_offset], ldb, &
		ierr, (ftnlen)1);
    }

/*     Permute the matrix to make it more nearly triangular */
/*     (Workspace: need 6*N + 2*N for permutation parameters) */

    ileft = 1;
    iright = *n + 1;
    iwrk = iright + *n;
    dggbal_("P", n, &a[a_offset], lda, &b[b_offset], ldb, &ilo, &ihi, &work[
	    ileft], &work[iright], &work[iwrk], &ierr, (ftnlen)1);

/*     Reduce B to triangular form (QR decomposition of B) */
/*     (Workspace: need N, prefer N*NB) */

    irows = ihi + 1 - ilo;
    icols = *n + 1 - ilo;
    itau = iwrk;
    iwrk = itau + irows;
    i__1 = *lwork + 1 - iwrk;
    dgeqrf_(&irows, &icols, &b[ilo + ilo * b_dim1], ldb, &work[itau], &work[
	    iwrk], &i__1, &ierr);

/*     Apply the orthogonal transformation to matrix A */
/*     (Workspace: need N, prefer N*NB) */

    i__1 = *lwork + 1 - iwrk;
    dormqr_("L", "T", &irows, &icols, &irows, &b[ilo + ilo * b_dim1], ldb, &
	    work[itau], &a[ilo + ilo * a_dim1], lda, &work[iwrk], &i__1, &
	    ierr, (ftnlen)1, (ftnlen)1);

/*     Initialize VSL */
/*     (Workspace: need N, prefer N*NB) */

    if (ilvsl) {
	dlaset_("Full", n, n, &c_b37, &c_b38, &vsl[vsl_offset], ldvsl, (
		ftnlen)4);
	i__1 = irows - 1;
	i__2 = irows - 1;
	dlacpy_("L", &i__1, &i__2, &b[ilo + 1 + ilo * b_dim1], ldb, &vsl[ilo 
		+ 1 + ilo * vsl_dim1], ldvsl, (ftnlen)1);
	i__1 = *lwork + 1 - iwrk;
	dorgqr_(&irows, &irows, &irows, &vsl[ilo + ilo * vsl_dim1], ldvsl, &
		work[itau], &work[iwrk], &i__1, &ierr);
    }

/*     Initialize VSR */

    if (ilvsr) {
	dlaset_("Full", n, n, &c_b37, &c_b38, &vsr[vsr_offset], ldvsr, (
		ftnlen)4);
    }

/*     Reduce to generalized Hessenberg form */
/*     (Workspace: none needed) */

    dgghrd_(jobvsl, jobvsr, n, &ilo, &ihi, &a[a_offset], lda, &b[b_offset], 
	    ldb, &vsl[vsl_offset], ldvsl, &vsr[vsr_offset], ldvsr, &ierr, (
	    ftnlen)1, (ftnlen)1);

    *sdim = 0;

/*     Perform QZ algorithm, computing Schur vectors if desired */
/*     (Workspace: need N) */

    iwrk = itau;
    i__1 = *lwork + 1 - iwrk;
    dhgeqz_("S", jobvsl, jobvsr, n, &ilo, &ihi, &a[a_offset], lda, &b[
	    b_offset], ldb, &alphar[1], &alphai[1], &beta[1], &vsl[vsl_offset]
	    , ldvsl, &vsr[vsr_offset], ldvsr, &work[iwrk], &i__1, &ierr, (
	    ftnlen)1, (ftnlen)1, (ftnlen)1);
    if (ierr != 0) {
	if (ierr > 0 && ierr <= *n) {
	    *info = ierr;
	} else if (ierr > *n && ierr <= *n << 1) {
	    *info = ierr - *n;
	} else {
	    *info = *n + 1;
	}
	goto L60;
    }

/*     Sort eigenvalues ALPHA/BETA and compute the reciprocal of */
/*     condition number(s) */
/*     (Workspace: If IJOB >= 1, need MAX( 8*(N+1), 2*SDIM*(N-SDIM) ) */
/*                 otherwise, need 8*(N+1) ) */

    if (wantst) {

/*        Undo scaling on eigenvalues before DELZTGing */

	if (ilascl) {
	    dlascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphar[1], 
		    n, &ierr, (ftnlen)1);
	    dlascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphai[1], 
		    n, &ierr, (ftnlen)1);
	}
	if (ilbscl) {
	    dlascl_("G", &c__0, &c__0, &bnrmto, &bnrm, n, &c__1, &beta[1], n, 
		    &ierr, (ftnlen)1);
	}

/*        Select eigenvalues */

	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    bwork[i__] = (*delctg)(&alphar[i__], &alphai[i__], &beta[i__]);
/* L10: */
	}

/*        Reorder eigenvalues, transform Generalized Schur vectors, and */
/*        compute reciprocal condition numbers */

	i__1 = *lwork - iwrk + 1;
	dtgsen_(&ijob, &ilvsl, &ilvsr, &bwork[1], n, &a[a_offset], lda, &b[
		b_offset], ldb, &alphar[1], &alphai[1], &beta[1], &vsl[
		vsl_offset], ldvsl, &vsr[vsr_offset], ldvsr, sdim, &pl, &pr, 
		dif, &work[iwrk], &i__1, &iwork[1], liwork, &ierr);

	if (ijob >= 1) {
/* Computing MAX */
	    i__1 = maxwrk, i__2 = (*sdim << 1) * (*n - *sdim);
	    maxwrk = max(i__1,i__2);
	}
	if (ierr == -22) {

/*            not enough real workspace */

	    *info = -22;
	} else {
	    rconde[1] = pl;
	    rconde[2] = pr;
	    rcondv[1] = dif[0];
	    rcondv[2] = dif[1];
	    if (ierr == 1) {
		*info = *n + 3;
	    }
	}

    }

/*     Apply permutation to VSL and VSR */
/*     (Workspace: none needed) */

    if (ilvsl) {
	dggbak_("P", "L", n, &ilo, &ihi, &work[ileft], &work[iright], n, &vsl[
		vsl_offset], ldvsl, &ierr, (ftnlen)1, (ftnlen)1);
    }

    if (ilvsr) {
	dggbak_("P", "R", n, &ilo, &ihi, &work[ileft], &work[iright], n, &vsr[
		vsr_offset], ldvsr, &ierr, (ftnlen)1, (ftnlen)1);
    }

/*     Check if unscaling would cause over/underflow, if so, rescale */
/*     (ALPHAR(I),ALPHAI(I),BETA(I)) so BETA(I) is on the order of */
/*     B(I,I) and ALPHAR(I) and ALPHAI(I) are on the order of A(I,I) */

    if (ilascl) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (alphai[i__] != 0.) {
		if (alphar[i__] / safmax > anrmto / anrm || safmin / alphar[
			i__] > anrm / anrmto) {
		    work[1] = (d__1 = a[i__ + i__ * a_dim1] / alphar[i__], 
			    abs(d__1));
		    beta[i__] *= work[1];
		    alphar[i__] *= work[1];
		    alphai[i__] *= work[1];
		} else if (alphai[i__] / safmax > anrmto / anrm || safmin / 
			alphai[i__] > anrm / anrmto) {
		    work[1] = (d__1 = a[i__ + (i__ + 1) * a_dim1] / alphai[
			    i__], abs(d__1));
		    beta[i__] *= work[1];
		    alphar[i__] *= work[1];
		    alphai[i__] *= work[1];
		}
	    }
/* L20: */
	}
    }

    if (ilbscl) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    if (alphai[i__] != 0.) {
		if (beta[i__] / safmax > bnrmto / bnrm || safmin / beta[i__] 
			> bnrm / bnrmto) {
		    work[1] = (d__1 = b[i__ + i__ * b_dim1] / beta[i__], abs(
			    d__1));
		    beta[i__] *= work[1];
		    alphar[i__] *= work[1];
		    alphai[i__] *= work[1];
		}
	    }
/* L30: */
	}
    }

/*     Undo scaling */

    if (ilascl) {
	dlascl_("H", &c__0, &c__0, &anrmto, &anrm, n, n, &a[a_offset], lda, &
		ierr, (ftnlen)1);
	dlascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphar[1], n, &
		ierr, (ftnlen)1);
	dlascl_("G", &c__0, &c__0, &anrmto, &anrm, n, &c__1, &alphai[1], n, &
		ierr, (ftnlen)1);
    }

    if (ilbscl) {
	dlascl_("U", &c__0, &c__0, &bnrmto, &bnrm, n, n, &b[b_offset], ldb, &
		ierr, (ftnlen)1);
	dlascl_("G", &c__0, &c__0, &bnrmto, &bnrm, n, &c__1, &beta[1], n, &
		ierr, (ftnlen)1);
    }

/* L40: */

    if (wantst) {

/*        Check if reordering is correct */

	lastsl = TRUE_;
	lst2sl = TRUE_;
	*sdim = 0;
	ip = 0;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    cursl = (*delctg)(&alphar[i__], &alphai[i__], &beta[i__]);
	    if (alphai[i__] == 0.) {
		if (cursl) {
		    ++(*sdim);
		}
		ip = 0;
		if (cursl && ! lastsl) {
		    *info = *n + 2;
		}
	    } else {
		if (ip == 1) {

/*                 Last eigenvalue of conjugate pair */

		    cursl = cursl || lastsl;
		    lastsl = cursl;
		    if (cursl) {
			*sdim += 2;
		    }
		    ip = -1;
		    if (cursl && ! lst2sl) {
			*info = *n + 2;
		    }
		} else {

/*                 First eigenvalue of conjugate pair */

		    ip = 1;
		}
	    }
	    lst2sl = lastsl;
	    lastsl = cursl;
/* L50: */
	}

    }

L60:

    work[1] = (doublereal) maxwrk;
    iwork[1] = liwmin;

    return 0;

/*     End of DGGESX */

} /* dggesx_ */
