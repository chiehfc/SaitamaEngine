#include "pch.h"
#include "MatrixStack.h"

MatrixStack::MatrixStack()
{
    m_stackSize = 16;
    m_base = new Matrix[m_stackSize];
    m_pos = 0;
    m_sp = &m_base[m_pos];

    *m_sp = Matrix::Identity;
}


MatrixStack::MatrixStack(int size)
{
    m_stackSize = size;
    m_base = new Matrix[m_stackSize];
    m_pos = 0;
    m_sp = &m_base[m_pos];
    *m_sp = Matrix::Identity;
}

MatrixStack::~MatrixStack(void)
{
    delete[] m_base;
}

void MatrixStack::Reset(void)
{
    m_pos = 0;
    m_sp = &m_base[m_pos];
    *m_sp = Matrix::Identity;
}

void MatrixStack::PushIdentity(void)
{
    Push();
    *m_sp = Matrix::Identity;
}


void MatrixStack::Push(void)
{
    if (m_pos < m_stackSize) {
        m_pos++;
        m_sp = &m_base[m_pos];
        *m_sp = m_base[m_pos - 1];
    }
}


void MatrixStack::Pop(void)
{
    if (m_pos > 0) {
        m_pos--;
        m_sp = &m_base[m_pos];
    }
}