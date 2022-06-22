class Numero {
    private int num;


    public Numero(){
        this.num = 0;
    }

    public synchronized String isPar(){
        return (this.num % 2) == 0 ? " 'Par' " : " 'Impar' ";
    }

    public int getNumero() {
        return this.num;
    }

    public void setNumero(int n) {
        this.num = n;
    }

    @Override
    public String toString() {
        return "Numero: " + num;
    }
}
