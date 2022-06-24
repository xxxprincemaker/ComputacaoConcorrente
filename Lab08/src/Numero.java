class Numero {
    private int num;


    public Numero(){
        this.num = 0;
    }

    public String isPar(){
        return (this.num % 2) == 0 ? " 'Par' " : " 'Impar' ";
    }

    public int getNumero() {
        return this.num;
    }

    public void setNumero(int n) {
        this.num = n;
    }

    public void imprimeNumero(){
        System.out.println("le.numero("+this.getNumero()+")");
    }

    public void imprimeParidade(){
        System.out.println("le.paridade("+this.getNumero()+","
                +this.isPar()+")");
    }

    @Override
    public String toString() {
        return "Numero: " + num;
    }
}
