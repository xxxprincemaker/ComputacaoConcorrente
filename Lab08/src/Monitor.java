class Monitor {
    private int leit, escr;

    Monitor() {
        this.leit = 0;
        this.escr = 0;
    }

    public synchronized void EntraLeitor(int id) {
        try {
            while (this.escr > 0) {
                System.out.println("le.leitorBloqueado(" + id + ")");
                wait();
            }
            this.leit++;
            System.out.println("le.leitorLendo(" + id + ")");
        } catch (InterruptedException e) {
        }
    }

    public synchronized void SaiLeitor(int id) {
        this.leit--;
        if (this.leit == 0)
            this.notify();
        System.out.println("le.leitorSaindo(" + id + ")");
    }

    public synchronized void EntraEscritor(int id) {
        try {
            while ((this.leit > 0) || (this.escr > 0)) {
                System.out.println("le.escritorBloqueado(" + id + ")");
                wait();
            }
            this.escr++;
            System.out.println("le.escritorEscrevendo(" + id + ")");
        } catch (InterruptedException e) {
        }
    }

    public synchronized void SaiEscritor(int id) {
        this.escr--;
        notifyAll();
        System.out.println("le.escritorSaindo(" + id + ")");
    }
}